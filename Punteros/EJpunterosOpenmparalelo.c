#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

#define N 10000000

int main() {
    int *arr = malloc(N * sizeof(int));

    // inicializar usando punteros
    for (int i = 0; i < N; i++) {
        *(arr + i) = i;
    }

    double inicio = omp_get_wtime();

// paralelizar usando OpenMP
    #pragma omp parallel for
    for (int i = 0; i < N; i++) {
        *(arr + i) = *(arr + i) * 2;
    }

    double fin = omp_get_wtime();

    printf("Tiempo paralelo (punteros + OpenMP): %f\n", fin - inicio);

    free(arr);
    return 0;
}