#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

#define N 10000000

int main() {
    int *arr = malloc(N * sizeof(int));

    // inicializar
    for (int i = 0; i < N; i++) {
        arr[i] = i;
    }

    double inicio = omp_get_wtime();

    // 🔹 procesamiento secuencial
    for (int i = 0; i < N; i++) {
        arr[i] = arr[i] * 2;
    }

    double fin = omp_get_wtime();

    printf("Tiempo secuencial: %f\n", fin - inicio);

    free(arr);
    return 0;
}