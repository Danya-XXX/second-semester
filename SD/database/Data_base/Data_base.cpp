//База данных студентов университета (id, ФИО, Направление,ступень образования, курс/имя научного руководителя, Средний балл)
//Использовать все простые типы, составные: 1)статические(массивы,записи, объединения).2)Динамические: А) нерекурсивные(массивы, записи) Б) Рекурсивные(деревья, линейные списки)
#define _CRT_SECURE_NO_WARNINGS
#include <string.h>
#include <Windows.h>
#include <stdio.h>
#include <locale.h>
#include "students.h"
#include "actions.h"

int main() {
	// Чтоб работали русские символы
	setlocale(LC_ALL, "Russian");
	SetConsoleCP(1251);
	SetConsoleOutputCP(1251);

	// Инициализация дерева студентов
	bin_Tree* root = NULL;
	
	// Открываем файл для чтения данных
	FILE* file = fopen("date_of_studentsnew.txt", "r");
	
	Student s;
	char info_time[100];
	char line[500];
	while (fgets(line, sizeof(line), file)) {
		int result = sscanf(line, "%d,%99[^,],%99[^,],%15[^,],%99[^,],%f",
			&s.id, s.fullname, s.specialty, s.level_of_education, info_time, &s.average_grade);
			if (strcmp(s.level_of_education, "Аспирантура") == 0) {
				strncpy(s.info.name, info_time, sizeof(s.info.name));
			}
			else {
				sscanf(info_time, "%d", &s.info.course);
			}
			root = insert(root, s); 
	}
	fclose(file);
	

	bool A = true;
	while (A) {
		printf("Выберите действие:\n1.Вставить пользователя.\n2.Найти пользователя.\n3.Удалить пользователя.\n4.Сохранить данные о пользователях в файл.\n5.Выйти\n6.Вывести всех студентов.\n");
		int action;
		scanf("%d", &action);

		switch (action) {
		case insert_student: 
		{
			Student s;
			printf("Добавьте данные студента:\n");

			printf("Введите ID: ");
			scanf("%d", &s.id);
			getchar(); 

			printf("Введите ФИО: ");
			fgets(s.fullname, sizeof(s.fullname), stdin);
			s.fullname[strcspn(s.fullname, "\n")] = 0; 
			

			
			printf("Введите направление: ");
			fgets(s.specialty, sizeof(s.specialty), stdin);
			s.specialty[strcspn(s.specialty, "\n")] = 0;
			

			printf("Введите ступень обучения(Бакалавриат/Аспирантура/Магистратура): ");
			fgets(s.level_of_education, sizeof(s.level_of_education), stdin);
			s.level_of_education[strcspn(s.level_of_education, "\n")] = 0;
			if (strcmp(s.level_of_education, "Аспирантура") == 0) {
				printf("Введите научного руководителя: ");
				fgets(s.info.name, sizeof(s.info.name), stdin);
				s.info.name[strcspn(s.info.name, "\n")] = 0;
			}
			else {
				printf("Введите курс обучения: ");
				scanf("%d", &s.info.course);
				getchar();
				
			}

			printf("Введите средний балл: ");
			scanf("%f", &s.average_grade);
			getchar();

			root = insert(root, s); 
			
		}
		break;
		case find_Student: 
		{
			
			int id;
			printf("Введите id: ");
			scanf("%d", &id);
			getchar();
			find_studentID(root, id);
		}
		break;
		case delete_student: 
		{
			int ID;
			printf("Введите id пользователя ");
			scanf("%d", &ID);
			root = del_student(root, ID);
			printf("Пользователь удален");
		}
		break;
		case add_data: {
			FILE* file = fopen("date_of_studentsnew.txt", "w+");
			
			savestudent(file, root);
			fclose(file);
			printf("Данные сохранены!\n");
			
			
		}
		break;
		case Exit: {
			char answer[10];
			printf("Хотите сохранить данные?\n");
			scanf(" %10[^\n]", answer);

			if (strcmp(answer, "Да") == 0 || strcmp(answer, "да") == 0) {
				FILE* file = fopen("date_of_studentsnew.txt", "w+");
				if (file) {
					savestudent(file, root);
					fclose(file);
					printf("Данные сохранены!\n");
				}
				else {
					printf("Ошибка сохранения данных!\n");
				}
			}
			A = false;
			break;
		}
		case print_studentsTree: 
			printTree(root);
			break;
		default:
			printf("Неверный выбор!\n");
			break;
		}
	}
	free_tree(root);
	return 0;
}
