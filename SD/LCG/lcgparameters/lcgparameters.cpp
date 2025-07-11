#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <locale.h>
#include <ctime>


#define ull unsigned long long
ull size = 1000000;
ull a = 6364136223846793005;
ull m = 18446744073709551615;
ull c = 1442695040888963407;


// Функция для вычисления факториала
ull factorial(int n) {
    unsigned long long result = 1;
    for (int i = 2; i <= n; i++) {
        result *= i;
    }
    return result;
}

// Функция для вычисления индекса перестановки
int permutation_index(ull* sequence, int t) {
    int f = 0;
    for (int r = t; r > 1; r--) {
        int max_index = 0;
        for (int i = 1; i < r; i++) {
            if (sequence[i] > sequence[max_index]) {
                max_index = i;
            }
        }
        f = r * f + max_index;
        ull temp = sequence[max_index];
        sequence[max_index] = sequence[r - 1];
        sequence[r - 1] = temp;
    }
    return f;
}
//Функция, создающая массив для будущего нахождения значения хи квадрата
int* make_mass(ull* sequence, int* counts,int k) {

    for (ull i = 0; i < size; i++) {
        double normalized = (double)sequence[i] / m;
        int index = (int)(normalized * k);
        counts[index]++;        
    }
    return counts;
}
//Функция, находящая хи квадрат последовательности
double find_hi2(int* counts, int num, ull Size) {
    double hi2 = 0.0;
    double np = (double)Size / num;
    for (int s = 0; s < num; s++) {
        hi2 += pow((counts[s] - np), 2) / np;
    }
    return hi2;
}

void print_result(double hi2, double* crit_value, int* result) {
    if (hi2 <= crit_value[0] || hi2 >= crit_value[5]) {
        //printf("Недостаточно случайные числа %f\n", hi2);
        result[0]++;
    }
    else if ((crit_value[0] < hi2 && hi2 <= crit_value[1]) || (crit_value[4] <= hi2 && hi2 < crit_value[5])) {
        //printf("Подозрительные числа %f\n", hi2);
        result[1]++;
    }
    else if ((crit_value[1] < hi2 && hi2 <= crit_value[2]) || (crit_value[3] <= hi2 && hi2 < crit_value[4])) {
        //printf("Почти подозрительные числа %f\n", hi2);
        result[2]++;
    }
    else if (crit_value[2] < hi2 && hi2 < crit_value[3]) {
        //printf("Подходящие числа %f\n", hi2);
        result[3]++;
    }

}

unsigned long long lcg(ull* seed, ull a, ull c, ull m) {
    return (*seed * a + c) % m;
}
void Power() {
    ull b = a - 1;
    ull g = a - 1;
    for (int i = 1; i <= 64; ++i) {
        if (b % m == 0) { 
            printf("Мощность равна: %d\n", i);
            return;
        }
        b *= (a - 1);  
    }
}


void Scatter() {
    ull seed = rand()*1234;
    
    ull* sequence = (ull*)malloc(size * sizeof(ull));
    sequence[0] = seed;

    for (ull i = 1; i < size; i++) {
        sequence[i] = lcg(&sequence[i - 1], a, c, m);

    }
    const int bins = 100; 
    int counts[bins];
    for (int i = 0; i < bins; i++) counts[i] = 0;

    for (ull i = 0; i < size; i++) {
        double normalized = (double)sequence[i] / m;  
        int index = (int)(normalized * bins);
        
        
        counts[index]++;
    
    }

    printf("Разброс по %d интервалам:\n", bins);
    for (int i = 0; i < bins; i++) {
        float otkl = float(counts[i]) / (size / bins);
        printf("Интервал %d: %d значений (отклонение на %f%%)\n", i, counts[i],otkl);
    }
}
void Period() {
    unsigned long long count = 0;

    long long period = 1;

    printf("Выполняется подсчет периода...\n");
    
    ull x = rand();
    count = lcg(&x,a,c,m);
    x = lcg(&x, a, c, m);
    while (count != lcg(&x,a,c,m)){
        x = lcg(&x, a, c, m);
        period++;
        //printf("\n%llu", period);
    }

    printf("Период равен %llu\n", period);
}


void Hi2() {
  
    double crit_value[6] = { 14.953, 18.493, 20.599,40.256, 43.773, 50.892 };
    int n = 100;
    int k = 31;
    int result[4] = {0};//i = 0 - недостаточно случайные числа, i = 1 -  подозрительные, i = 2 - почти подозрительные, i = 3 - подходящие
    while (n) {
        
       
        ull* sequence = (ull*)malloc(size * sizeof(ull));
        sequence[0] = rand();

        for (ull i = 1; i < size; i++) {
            sequence[i] = lcg(&sequence[i - 1], a, c, m);
        }
        int counts[100] = {0};
        make_mass(sequence, counts, k);
        double hi2 = find_hi2(counts,k, size);
        print_result(hi2, crit_value, result);
        
        free(sequence);
        n--;
    }
    printf("Было сгенерировано 100 последовательностей, из них\n");
    printf("Подходщие - %d\nПочти подозрительные - %d\nПодозрительные - %d\nНедостаточно случайные - %d\n", result[3], result[2], result[1], result[0]);
    if ((result[2] + result[1] + result[0]) / 100 > 2 / 3) { printf("Плохие параметры, т.к частота подозрительных результатов больше 2/3(%d)\n", (result[2] + result[1] + result[0]) / 100); }
    else { printf("Подходящие параметры\n"); }
}
void Permutations() {
    double crit_value[6] = { 0.554, 1.145, 1.610, 9.236, 11.070, 15.086 };
    int iter = 100;
    int t = 3;
    int result[4] = { 0 };
    while (iter) {
       
        
        ull* sequence = (ull*)malloc(size * sizeof(ull));
        sequence[0] = rand();

        for (ull i = 1; i < size; i++) {
            sequence[i] = lcg(&sequence[i - 1], a, c, m);

        }
      


        ull n = size / t;
        int k = factorial(t);
        int counts[7] = { 0 };

        for (ull i = 0; i < n; i++) {
            ull group[4] = { 0 };
            for (int j = 0; j < t; j++) {
                group[j] = sequence[i * t + j];
            }
            int index = permutation_index(group, t);
            counts[index]++;

        }
        double hi2 = find_hi2(counts,k, n);
        
        
        print_result(hi2, crit_value, result);

        iter--;
        free(sequence);
    }
    printf("Было сгенерировано 100 последовательностей, из них\n");
    printf("Подходщие - %d\nПочти подозрительные - %d\nПодозрительные - %d\nНедостаточно случайные - %d\n", result[3], result[2], result[1], result[0]);
    if ((result[2] + result[1] + result[0]) / 100 > 2 / 3) { printf("Плохие параметры, т.к частота подозрительных результатов больше 2/3(%d)\n", (result[2] + result[1] + result[0]) / 100); }
    else { printf("Подходящие параметры\n"); }
    
}

int main() {
    setlocale(LC_ALL, "rus");
    
    
    while (1) {
        int action;
        printf("Выберите критерий:\n1.Мощность.\n2.Разброс.\n3.Период.\n4.Хи-квадрат.\n5.Перестановки.\n");
        scanf("%d", &action);
        
        switch (action)
        {

        case 1:
            Power();
            break;
        case 2:
            Scatter();
        
            break;
        case 3:
            Period();
            
            break;
        case 4:
            Hi2();
            
            break;
        case 5:
            Permutations();
            
            break;
        default:
            printf("Неверный номер критерия.\n");
            
            break;
        }
        
    }
  
 
    return 0;
}

