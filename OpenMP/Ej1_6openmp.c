#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

#define N 10000000

int main() {

    double *A = malloc(N * sizeof(double));
    double *B = malloc(N * sizeof(double));
    double resultado = 0.0;

    // inicializar vectores con valores aleatorios
    for (int i = 0; i < N; i++) {
        A[i] = rand() / (double)RAND_MAX;
        B[i] = rand() / (double)RAND_MAX;
    }

    // ---- VERSION SIN SIMD ----
    double inicio = omp_get_wtime();

    for (int i = 0; i < N; i++) {
        resultado += A[i] * B[i];
    }

    double fin = omp_get_wtime();
    printf("Resultado sin SIMD: %f\n", resultado);
    printf("Tiempo sin SIMD: %f segundos\n", fin - inicio);

    // ---- VERSION CON SIMD ----
    resultado = 0.0;
    inicio = omp_get_wtime();

    // TODO: agregar directiva simd con reduction
    #pragma omp simd reduction(+:resultado)
    for (int i = 0; i < N; i++) {
        resultado += A[i] * B[i];
    }

    fin = omp_get_wtime();
    printf("Resultado con SIMD: %f\n", resultado);
    printf("Tiempo con SIMD: %f segundos\n", fin - inicio);

    free(A);
    free(B);
    return 0;
}