#include <stdio.h>
#include <stdlib.h>

void write_dot(FILE* fname, int** mas, int size)
{
	fprintf(fname, "graph G{\n");

	for (int i = 0; i < size; i++)
		fprintf(fname, "%d;\n", i);

	for (int j = 0; j < size; j++) {
		for (int i = j; i < size; i++)
			for (int k = 0; k < mas[i][j]; k++)
				fprintf(fname, "%d -- %d;\n", j, i);
	}
	fprintf(fname, "}");
	fclose(fname);
}

void free_2mas(int** mas, int size) {
	for (int i = 0; i < size; i++)
		free(mas[i]);
	free(mas);
	mas = NULL;
}

void enter_size(int* size) {
	free(size);
	size = (int*)malloc(sizeof(size));
	printf("\nEnter the width: ");
	scanf("%d", size);																			//пользователь вводит размер матрицы

}

void make_graph(int*** mas, int size) {															// создание 2мерного массива
																								// для создания 2мерного массива необходимо чтобы каждая ячека одномерного массива была указателем на элемент матрицы
	*mas = (int**)malloc(sizeof(*mas) * size);													// пройдя по адресу *mas получаем указатель на int
	for (int i = 0; i < size; i++) 																// выделяя память каждому указателю мы получаем массивы в каждой ячейке памяти
		(*mas)[i] = (int*)malloc(sizeof(**mas) * size);											// пройдя по адресу **mas получаем int
}

void fill_graph(int*** mas, int size) {

	printf("\nEnter the rows of your matrix. With pressing \"ENTER\" you will change column. If do not specify a number it will be '0'.\n");
	getchar();																					// ловит ENTER при вводе размера

	char* s, c;
	s = (char*)malloc(sizeof(s));

	for (int i = 0, m = 1, j = 0; j < size; j++) {												// заполняем массив числами 

		while ((c = getchar()) != '\n') {

			if (c < '0' && c > '9' && c != ' ') {
				printf("ERROR: the entered values is wrong!\n");
				exit(1);
			}

			if (c != ' ') {
				s[m - 1] = c;

				s = (char*)realloc(s, sizeof(s) * (++m));
			}

			else {
				s[m - 1] = '\0';

				if (i >= size) {
					printf("ERROR: numders that you entered more than entered width!\n ");
					exit(1);
				}

				(*mas)[i][j] = atoi(s);
				i++;
				m = 1;

				free(s);
				s = NULL;
				s = (char*)malloc(sizeof(s));

			}
		}

		s[m - 1] = '\0';

		if (i >= size) {
			printf("ERROR: numders that you entered more than entered width!\n ");
			exit(1);
		}

		(*mas)[i][j] = atoi(s);

		if (i < size) {																			 // замена числа на 0 если оно не введено
			++i;																				 // след элемент
			while (i < size)
				(*mas)[i++][j] = 0;
		}

		i = 0;
		m = 1;

		free(s);
		s = NULL;
		s = (char*)malloc(sizeof(s));
	}

	free(s);
	s = NULL;

}

int main(void) {

	int** first = NULL, ** second = NULL, ** cross = NULL, * size;
	size = (int*)malloc(sizeof(size));

	enter_size(size);																	   	//Ввод размера массива
	make_graph(&first, *size);																//Создание матрицы смежности графа
	fill_graph(&first, *size);																//Заполнение матрицы смежности

	int min = *size;																		//Размер первой введенной матрицы становится минимальным


	enter_size(size);
	make_graph(&second, *size);
	fill_graph(&second, *size);

	int max;
	if (min > *size) {																		//Нахождение минимального(для создания матрицы пересечения графов) и максимального (для дальнейшей очистки массивов) размеров введеных графов
		max = min;
		min = *size;
	}
	else
		max = *size;

	make_graph(&cross, min);

	for (int j = 0; j < min; j++)															//Нахождение пересечения графов
		for (int i = 0; i < min; i++) {														//Если конкретная ячейка матриц графов не пуста(!=0),то графы между вершинами имеют общее ребро или ребра, иначе если хотя бы у одного графа нет там ребра, то и нет пересечения вершин
			if ((first[i][j] != 0) && (second[i][j] != 0)) {
					cross[i][j] = (first[i][j] > second[i][j]) ? second[i][j] : first[i][j];
			}
			else
				cross[i][j] = 0;
		}

	//for (int j = 0; j < min; j++)															// выводим заполненный массив пересечения
	//	for (int i = 0; i < min; i++)
	//		printf("%d\t%s", cross[i][j], ((i == min - 1) ? "\n" : ""));

	FILE* fout;
	fout = fopen("graph.gv", "w");
	if (fout == NULL) {
		perror("fopen()");
		return EXIT_FAILURE;
	}

	write_dot(fout, cross, min);
	system("dot -Tjpg graph.gv -o file.jpg");

	free_2mas(first, max);
	free_2mas(second, max);
	free_2mas(cross, min);
	free(size);
	size = NULL;
	
	printf("The Intersection of two Graphs was complete.\n");
	return 0;
}
