#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <omp.h>

#define N 10000000

int main() {

    double *v = malloc(N * sizeof(double));
    double suma = 0.0;
    double norma = 0.0;

    // 1. Inicializar el vector con valores aleatorios
    for (int i = 0; i < N; i++) {
        v[i] = rand() / (double)RAND_MAX;
    }

    // ---- VERSION SIN SIMD ----
    double inicio = omp_get_wtime();

    #pragma omp parallel for reduction(+:suma)
    for (int i = 0; i < N; i++) {
        suma += v[i] * v[i];
    }

    norma = sqrt(suma);

    double fin = omp_get_wtime();
    printf("Norma sin SIMD: %f\n", norma);
    printf("Tiempo sin SIMD: %f segundos\n", fin - inicio);

    // ---- VERSION CON SIMD ----
    suma = 0.0;
    norma = 0.0;

    inicio = omp_get_wtime();


    for (int i = 0; i < N; i++) {
        suma += v[i] * v[i];
    }

        norma = sqrt(suma);

    fin = omp_get_wtime();
    printf("Norma con SIMD: %f\n", norma);
    printf("Tiempo con SIMD: %f segundos\n", fin - inicio);

    free(v);
    return 0;
}