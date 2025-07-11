//Проверить гипотизу, что сортировка линейного списка быстрее сортировки вставками массива
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <time.h>
#include <intrin.h>
#include <stdlib.h>
#include <locale.h>
#define CPU_frequency (long)1900000000
#define ull unsigned long long
ull a = 6364136223846793005;
ull m = 18446744073709551615;
ull c = 1442695040888963407;
void lcg(ull* arr,ull size) {
    arr[0] = rand();
    for (int i = 1; i< size; i++) {
        arr[i] = (a * arr[i - 1] + c) % m;
    }
    
}
// Структура узла списка
typedef struct linelist {
    int number;
    struct linelist* next;
    struct linelist* pre;
} linelist;

// Функция для создания предварительно выделенного списка
linelist* create_preallocated_list(ull* values, ull size, linelist* memory_pool) {
    linelist* head = &memory_pool[0];
    head->number = values[0];
    head->pre = NULL;

    linelist* current = head;
    for (int i = 1; i < size; i++) {
        memory_pool[i].number = values[i];
        memory_pool[i].pre = current;
        memory_pool[i].next = NULL;
        current->next = &memory_pool[i];
        current = current->next;
    }
    return head;
}

// Сортировка массива (Insertion Sort)
void sort(ull* seq, int size) {
    
    for (int i = 1; i < size; i++) {

        int x = seq[i];
        int j = i - 1;
        
        while (j >= 0 && seq[j] > x) {
            seq[j + 1] = seq[j];
            j--;
        }
        
        seq[j + 1] = x;
    }

}

// Сортировка списка заменой значений (Insertion Sort)
void sort_list_number(linelist* node) {
    linelist* i = node->next;
    while (i != NULL) {
        int x = i->number;
        linelist* j = i->pre;
        while (j != NULL && j->number > x) {
            j->next->number = j->number;
            j = j->pre;
        }
        if (j == NULL)
            node->number = x;
        else
            j->next->number = x;
        i = i->next;
    }
}

// Сортировка списка заменой указателей
void sort_list(linelist** node) {
    if (*node == NULL || (*node)->next == NULL)
        return;

    linelist* i = (*node)->next;
    while (i != NULL) {
        linelist* x = i;
        linelist* j = i->pre;
        i = i->next;

        if (j != NULL && j->number <= x->number)
            continue;

        // Отключаем x из текущего места
        if (x->pre)
            x->pre->next = x->next;
        if (x->next)
            x->next->pre = x->pre;

        // Ищем место для вставки
        while (j != NULL && j->number > x->number)
            j = j->pre;

        if (j == NULL) {
            // Вставка в начало
            x->next = *node;
            x->pre = NULL;
            (*node)->pre = x;
            *node = x;
        }
        else {
            // Вставка после j
            x->next = j->next;
            if (j->next)
                j->next->pre = x;
            j->next = x;
            x->pre = j;
        }
    }
}

// Генерация случайных чисел
void generate(ull* arr, int n) {
    
    for (int i = 0; i < n; i++)
        arr[i] = rand();
    
}

int main() {
    setlocale(LC_ALL, "rus");

    int mass_size[7] = { 100,1000,10000, 30000, 50000, 100000 };
   
    linelist* memory_pool1 = (linelist*)malloc(10000000 * sizeof(linelist));
    linelist* memory_pool2 = (linelist*)malloc(10000000 * sizeof(linelist));
    
    ull* sequence = (ull*)malloc(10000000 * sizeof(ull));

    for (int i = 0; i < 6; i++) {
        int size = mass_size[i];
        printf("Количество чисел: %d\n", size);
        unsigned long long median_arrey = 0;
        unsigned long long median_list1 = 0;
        unsigned long long median_list2 = 0;
        unsigned long long median_takts_put_seq_in_list = 0;
        unsigned long long generate_time = 0;
        
        
        unsigned __int64 start = __rdtsc();
        for (int j = 0; j < 10; j++) {
            unsigned __int64 generate_time_start = __rdtsc();
            lcg(sequence, size);
            unsigned __int64 generate_time_end = __rdtsc();
            generate_time += (generate_time_end - generate_time_start);
            // Создаем списки из предвыделенной памяти
            unsigned __int64  put_seq_in_list_start= __rdtsc();
            linelist* node1 = create_preallocated_list(sequence, size, memory_pool1);
            linelist* node2 = create_preallocated_list(sequence, size, memory_pool2);
            unsigned __int64  put_seq_in_list_end = __rdtsc();
            median_takts_put_seq_in_list += (put_seq_in_list_end - put_seq_in_list_start);
            //printf("Такты и время на внесение чисел в линейный список %llu", put_seq_in_list_end - put_seq_in_list_start);
            // Тест сортировки массива
            unsigned __int64 start1 = __rdtsc();
            sort(sequence, size);
            unsigned __int64 end1 = __rdtsc();
            median_arrey += (end1 - start1);
            //printf("Сортировка массива:                  %llu тактов\n", end1 - start1);

            // Тест сортировки списка заменой значений
            unsigned __int64 start2 = __rdtsc();
            sort_list_number(node1);
            unsigned __int64 end2 = __rdtsc();
            median_list1 += (end2 - start2);
            //printf("Сортировка списка изменением полей:  %llu тактов\n", end2 - start2);

            // Тест сортировки списка заменой указателей
            unsigned __int64 start3 = __rdtsc();
            sort_list(&node2);
            unsigned __int64 end3 = __rdtsc();
            median_list2 += (end3 - start3);
            //printf("Сортировка списка сменой указателей: %llu тактов\n", end3 - start3);

            //printf("-------------------\n");
        }
        unsigned __int64 end = __rdtsc();
        median_arrey /= 10; median_list1 /= 10; median_list2 /= 10; median_takts_put_seq_in_list /= 10; generate_time /= 10;
        printf("Средняя частота и время за 10 измерений\n");
        printf("Сортировка массива:                      %llu тактов (%f секунд)\
              \nСортировка списка изменением полей:      %llu тактов (%f секунд)\
              \nСортировка списка изменением указателей: %llu тактов (%f секунд)\
              \nОбщее потраченное время на опыт                         %f секунд\
              \nВремя, потраченное на создание линейных списков         %f секунд\
              \nВремя, потраченное на генерацию последовательности      %f секунд\n",\
                median_arrey, (double)median_arrey / (double)CPU_frequency,  median_list1, (double)median_list1 / (double)CPU_frequency,\
                median_list2,(double)median_list2 / (double)CPU_frequency,(double)(end-start)/double(CPU_frequency), (double)median_takts_put_seq_in_list/(double)CPU_frequency, (double)generate_time/(double)CPU_frequency);
        printf("-------------------\n");
        // Освобождаем память
        
    }
    free(memory_pool1);
    free(memory_pool2);
    free(sequence);
    return 0;
}