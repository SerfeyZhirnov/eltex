#include <stdio.h>

#define N 4

void Task1() {
  printf("Task1:\n");
  int matrix[N][N];
  for (int i = 0, counter = 1; i < N; ++i) {
    for (int j = 0; j < N; ++j, ++counter) {
      matrix[i][j] = counter;
      printf("%d ", matrix[i][j]);
    }
    printf("\n");
  }
  printf("\n");
}

void Task2() {
  printf("Task2:\n");
  int matrix[N];
  for (int i = 0, counter = 1; i < N; ++i, ++counter) {
    matrix[i] = counter;
    printf("%d ", matrix[i]);
  }
  printf("\n");

  const int middle = (const int)(N / 2);
  for (int i = 0; i < N; ++i) {
    if (i < middle) {
      matrix[i] += matrix[N - 1 - i];
      matrix[N - 1 - i] = matrix[i] - matrix[N - 1 - i];
      matrix[i] -= matrix[N - 1 - i];
    }
    printf("%d ", matrix[i]);
  }
  printf("\n\n");
}

void Task3() {
  printf("Task3:\n");
  int matrix[N][N];
  for (int i = 0; i < N; ++i) {
    for (int j = 0; j < N; ++j) {
      matrix[i][j] = (j >= N - i - 1);
      printf("%d ", matrix[i][j]);
    }
    printf("\n");
  }
  printf("\n");
}

void Task4() {
  printf("Task4:\n");
  int matrix[N][N];

  int row_start = 0, row_end = N - 1;
  int col_start = 0, col_end = N - 1;
  int counter = 0;

  while (row_start <= row_end && col_start <= col_end) {
    for (int i = col_start; i <= col_end; ++i) {
      matrix[row_start][i] = ++counter;
    }

    for (int i = row_start + 1; i <= row_end; ++i) {
      matrix[i][col_end] = ++counter;
    }

    if (row_start < row_end && col_start < col_end) {
      for (int i = col_end - 1; i >= col_start; --i) {
        matrix[row_end][i] = ++counter;
      }

      for (int i = row_end - 1; i > row_start; --i) {
        matrix[i][col_start] = ++counter;
      }
    }
    ++row_start, --row_end;
    ++col_start, --col_end;
  }

  for (int i = 0; i < N; ++i) {
    for (int j = 0; j < N; ++j) {
      printf("%02d ", matrix[i][j]);
    }
    printf("\n");
  }
}

int main() {
  Task1();
  Task2();
  Task3();
  Task4();

  return 0;
}