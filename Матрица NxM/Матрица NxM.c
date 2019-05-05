#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <math.h>
#include <string.h>
#include <stdlib.h>

//Структура содержит указатель на двумерный массив, значение числа строк(n) и столбцов(m)
typedef struct Matrix_NxM {
	float** data;
	int n;
	int m;
} matrix;

//Функция выделения памяти под матрицу NxM и инициализации структуры
matrix MemMatrix(int n, int m) {
	matrix resultmat;
	
	resultmat.n = n;
	resultmat.m = m;
	resultmat.data = (float**) malloc(sizeof(float*) * n);

	for (int i = 0; i < n; i++) {
		resultmat.data[i] = (float*) malloc(sizeof(float) * m);
	}
	return resultmat;
}

//Функция освобоождения памяти для матрицы из mat
void freeMatrix(matrix mat) {
	for (int i = 0; i < mat.n; i++) {
		free(mat.data[i]);
	}
	free(mat.data);
}

//Функция инициализации матрицы из mat
matrix InitMatrix(matrix mat) {
	for (int i = 0; i < mat.n; i++) {
		for (int j = 0; j < mat.m; j++) {
			printf("Input %d row %d column: \n", i + 1, j + 1);
			scanf_s("%f", &mat.data[i][j]);
		}
	}
	return mat;
}

//Функция вывода матрицы из mat
void ReturnMatrix(matrix mat) {
	FILE *fp = fopen("Result.txt", "w");
	printf("Final Matrix: \n");
 	for (int i = 0; i < mat.n; i++) {
		for (int j = 0; j < mat.m; j++) {
			printf("%.1f  ", mat.data[i][j]);
			fprintf(fp, "%.1f ", mat.data[i][j]);
		}
		printf("\n");
		putc('\n', fp);
	}
	fclose(fp);
}

//Функция суммирования матриц из mat1 и mat2
matrix SummMatrix(matrix mat1, matrix mat2) {

	//Проверка на равные размеры матриц
	if (mat1.n != mat2.n || mat1.m != mat2.m) {
		printf("Can't get the result!");
		return MemMatrix(0, 0);
	}

	//Суммирование
	matrix resultmat = MemMatrix(mat1.n, mat1.m);

	for (int i = 0; i < mat1.n; i++) {
		for (int j = 0; j < mat1.m; j++) {
			resultmat.data[i][j] = mat1.data[i][j] + mat2.data[i][j];
		}
	}

	return resultmat;
}

//Функция произведения матриц из mat1 и mat2
matrix MultMatrix(matrix mat1, matrix mat2) {

	//Проверка на соответствие размеров матриц
	if (mat1.m != mat2.n) {
		printf("Can't get the result!");
		return MemMatrix(0, 0);
	}

	float temp;
	matrix resultmat = MemMatrix(mat1.n, mat2.m);

	for (int i = 0; i < mat1.n; i++) {
		for (int j = 0; j < mat2.m; j++) {
			temp = 0;

			//Суммирование произведений элементов из i-й строки и j-го столбца в переменную temp
			for (int k = 0; k < mat1.m; k++) {
				temp += mat1.data[i][k] * mat2.data[k][j]; 
			}
			resultmat.data[i][j] = temp;
		}
	}
	
	return resultmat;
}

//Функция создания матрицы из дополнительных миноров матрицы из mat
matrix addMatrix(matrix mat, int i, int j) {

	//Долго объяснять. Это работает :)
	int s = 0, t = 0;
	float temp;
	matrix tempmat = MemMatrix((mat.n)-1,(mat.m)-1);
	
	for (int k = 0; k < tempmat.n; k++) {
		if (k == i) {
			s++;
		}

		for (int l = 0; l < tempmat.m; l++) {
			if (l == j) {
				t++;
			}

			tempmat.data[k][l] = mat.data[k+s][l+t];
		}
		t = 0;
	}

	return tempmat;
}

//Функция вычисления определителя матрицы из mat
float DetMatrix(matrix mat) {
	float det = 0;
	matrix tempmat;

	if (mat.m == 1) {
		return mat.data[0][0];
	}

	//Разложение определителя по первой строке
	for (int j = 0; j < mat.m; j++) {
		tempmat = addMatrix(mat, 0, j);
		det += pow(-1.0, (double)j) * (mat.data[0][j]) * (DetMatrix(tempmat));
		freeMatrix(tempmat);
	}
	return det;
}

//Функция транспонирования матрицы
matrix TransMatrix(matrix mat) {
	float temp;
	for (int i = 0; i < mat.n; i++) {
		for (int j = i + 1; j < mat.m; j++) {
			temp = mat.data[i][j];
			mat.data[i][j] = mat.data[j][i];
			mat.data[j][i] = temp;
		}
	}
	return mat;
}

//Функция создания обратной матрицы к матрице из mat
matrix InvMatrix(matrix mat) {

	//Проверка на вырожденность и неквадратность
	if (mat.n != mat.m || DetMatrix(mat) == 0) {
		printf("Can't get the result!");
		return MemMatrix(0, 0);
	}

	matrix addmat;
	float det = DetMatrix(mat);	//Вычисление определителя данной матрицы
	matrix resultmat = MemMatrix(mat.n, mat.m);

	//Запись в новую матрицу алгебраических дополнений
	for (int i = 0; i < mat.n; i++) {
		for (int j = 0; j < mat.m; j++) {
			addmat = addMatrix(mat, i, j);
			resultmat.data[i][j] = pow(-1.0, (double)i + j) * DetMatrix(addmat) / det;
			freeMatrix(addmat);
		}
	}

	resultmat = TransMatrix(resultmat);	//Транспонирование
	return resultmat;
}

int main() {
	char ch;
	do {
		//Определение типа операции
		 FILE *fp;
		char iotype;
		printf("Input type of insertion(s - stdio/f - file): ");
		scanf_s("%c", &iotype);
		getchar();

		if (iotype == 'S' || iotype == 's') {
			fp = stdin;
		}
		else if (iotype == 'F' || iotype == 'f') {
			fp = fopen("Matrix.txt", "r");
		}
		else {
			printf("Mistake :)");
			fp = NULL;
		}


		if (fp == NULL) {
			printf("File opening error!\n");
		}
	
		char type;
		printf("Input type of operation (S - Summ/M - Multiplication/D - Determinant/I - Invertible): ");
		scanf_s("%c", &type);

		if (type == 'S' || type == 's') {
			matrix mat1, mat2, resultmat;
			int n1, m1, n2, m2;

			//Считывание размеров матриц
			printf("Input length and width of 1-st matrix: \n");
			scanf_s("%d %d", &n1, &m1);
			printf("Input length and width of 2-nd matrix: \n");
			scanf_s("%d %d", &n2, &m2);

			//Создание матриц
			printf("1-st matrix initialization: \n");
			mat1 = MemMatrix(n1, m1);
			mat1 = InitMatrix(mat1);

			printf("2-nd matrix initialization: \n");
			mat2 = MemMatrix(n2, m2);
			mat2 = InitMatrix(mat2);

			//Вычисления
			resultmat = SummMatrix(mat1, mat2);
			ReturnMatrix(resultmat);

			//Освобождение памяти
			freeMatrix(mat1);
			freeMatrix(mat2);
			freeMatrix(resultmat);
		}
		else if (type == 'M' || type == 'm') {
			matrix mat1, mat2, resultmat;
			int n1, m1, n2, m2;

			//Считывание размеров матриц
			printf("Input length and width of 1-st matrix: \n");
			scanf_s("%d %d", &n1, &m1);
			printf("Input length and width of 2-nd matrix: \n");
			scanf_s("%d %d", &n2, &m2);

			//Создание матриц
			printf("1-st matrix initialization: \n");
			mat1 = MemMatrix(n1, m1);
			mat1 = InitMatrix(mat1);

			printf("2-nd matrix initialization: \n");
			mat2 = MemMatrix(n2, m2);
			mat2 = InitMatrix(mat2);

			//Вычисления
			resultmat = MultMatrix(mat1, mat2);
			ReturnMatrix(resultmat);

			//Освобождение памяти
			freeMatrix(mat1);
			freeMatrix(mat2);
			freeMatrix(resultmat);
		}
		else if (type == 'D' || type == 'd') {
			matrix mat;
			int n, m;
			float det;

			//Считывание размеров матрицы
			printf("Input length and width of  matrix : \n");
			scanf_s("%d %d", &n, &m);

			//Создание матрицы
			printf("Matrix initialization: \n");
			mat = MemMatrix(n, m);
			mat = InitMatrix(mat);

			//Проверка матрицы на неквадратность
			if (mat.n != mat.m) {
				printf("Can't get the result!");
			}
			else {
				//Вычисления
				det = DetMatrix(mat);
				printf("%.1f", det);
			}

			//Освобождение памяти
			freeMatrix(mat);

			FILE *fp = fopen("Result.txt", "w");
			fprintf(fp, "%.1f ", det);
			fclose(fp);
		}
		else if (type == 'I' || type == 'i') {
			matrix mat, resultmat;
			int n, m;

			//Считывание размеров матрицы
			printf("Input length and width of  matrix : \n");
			scanf_s("%d %d", &n, &m);

			//Создание матрицы
			printf("Matrix initialization: \n");
			mat = MemMatrix(n, m);
			mat = InitMatrix(mat);

			//Вычисления
			resultmat = InvMatrix(mat);
			ReturnMatrix(resultmat);

			//Освобождение памяти
			freeMatrix(mat);
			freeMatrix(resultmat);
		}
		else {
			printf("Mistake, sweetheart :)");
		}
		
		

		//Повтор программы
		printf("\nDo you wish to restart program? (y/n)\n");
		getchar();
		scanf_s("%c", &ch);
		getchar();
	} while (ch == 'y' || ch == 'Y');

	return 0;
}