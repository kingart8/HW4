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
	scanf("%d", size);																			//������������ ������ ������ �������

}

void make_graph(int*** mas, int size) {															// �������� 2������� �������
																								// ��� �������� 2������� ������� ���������� ����� ������ ����� ����������� ������� ���� ���������� �� ������� �������
	*mas = (int**)malloc(sizeof(*mas) * size);													// ������ �� ������ *mas �������� ��������� �� int
	for (int i = 0; i < size; i++) 																// ������� ������ ������� ��������� �� �������� ������� � ������ ������ ������
		(*mas)[i] = (int*)malloc(sizeof(**mas) * size);											// ������ �� ������ **mas �������� int
}

void fill_graph(int*** mas, int size) {

	printf("\nEnter the rows of your matrix. With pressing \"ENTER\" you will change column. If do not specify a number it will be '0'.\n");
	getchar();																					// ����� ENTER ��� ����� �������

	char* s, c;
	s = (char*)malloc(sizeof(s));

	for (int i = 0, m = 1, j = 0; j < size; j++) {												// ��������� ������ ������� 

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

		if (i < size) {																			 // ������ ����� �� 0 ���� ��� �� �������
			++i;																				 // ���� �������
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

	enter_size(size);																	   	//���� ������� �������
	make_graph(&first, *size);																//�������� ������� ��������� �����
	fill_graph(&first, *size);																//���������� ������� ���������

	int min = *size;																		//������ ������ ��������� ������� ���������� �����������


	enter_size(size);
	make_graph(&second, *size);
	fill_graph(&second, *size);

	int max;
	if (min > *size) {																		//���������� ������������(��� �������� ������� ����������� ������) � ������������� (��� ���������� ������� ��������) �������� �������� ������
		max = min;
		min = *size;
	}
	else
		max = *size;

	make_graph(&cross, min);

	for (int j = 0; j < min; j++)															//���������� ����������� ������
		for (int i = 0; i < min; i++) {														//���� ���������� ������ ������ ������ �� �����(!=0),�� ����� ����� ��������� ����� ����� ����� ��� �����, ����� ���� ���� �� � ������ ����� ��� ��� �����, �� � ��� ����������� ������
			if ((first[i][j] != 0) && (second[i][j] != 0)) {
					cross[i][j] = (first[i][j] > second[i][j]) ? second[i][j] : first[i][j];
			}
			else
				cross[i][j] = 0;
		}

	//for (int j = 0; j < min; j++)															// ������� ����������� ������ �����������
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
