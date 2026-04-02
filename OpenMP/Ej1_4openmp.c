#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

void multiplicar(int N) {

    double **A = malloc(N * sizeof(double *));
    double **B = malloc(N * sizeof(double *));
    double **C = malloc(N * sizeof(double *));

    for (int i = 0; i < N; i++) {
        A[i] = malloc(N * sizeof(double));
        B[i] = malloc(N * sizeof(double));
        C[i] = malloc(N * sizeof(double));
    }

    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            A[i][j] = rand() / (double)RAND_MAX;
            B[i][j] = rand() / (double)RAND_MAX;
            C[i][j] = 0.0;
        }
    }

    printf("Multiplicando matrices %dx%d...\n", N, N);
    double inicio = omp_get_wtime();

    #pragma omp parallel for collapse(2)
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            for (int k = 0; k < N; k++) {
                C[i][j] += A[i][k] * B[k][j];
            }
        }
    }

    double fin = omp_get_wtime();
    printf("Tiempo N=%d: %f segundos\n\n", N, fin - inicio);

    for (int i = 0; i < N; i++) {
        free(A[i]); free(B[i]); free(C[i]);
    }
    free(A); free(B); free(C);
}

int main() {
    printf("Hilos disponibles: %d\n\n", omp_get_max_threads());
    multiplicar(500);
    multiplicar(1000);
    multiplicar(2000);
    return 0;
}