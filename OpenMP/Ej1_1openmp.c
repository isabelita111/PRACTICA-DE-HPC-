#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

#define N 1000000

int main() {

    int *arr = malloc(N * sizeof(int));
    long long suma = 0;

    // inicializar arreglo
    for (int i = 0; i < N; i++) {
        arr[i] = 1;
    }

    printf("Hilos disponibles: %d\n", omp_get_max_threads());

    double inicio = omp_get_wtime();

    #pragma omp parallel for
    for (int i = 0; i < N; i++) {
        #pragma omp atomic
        suma += arr[i];
    }

    double fin = omp_get_wtime();

    printf("Suma: %lld\n", suma);
    printf("Tiempo: %f\n", fin - inicio);

    free(arr);
    return 0;
}