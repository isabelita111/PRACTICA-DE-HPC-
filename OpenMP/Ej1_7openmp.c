#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <omp.h>

#define N      5      // tamaño del sistema
#define ITER   1000   // iteraciones maximas
#define TOL    1e-6   // tolerancia de convergencia

int main() {

    // matriz A y vector b
    double A[N][N] = {
        {10, 1, 1, 1, 1},
        {1, 10, 1, 1, 1},
        {1, 1, 10, 1, 1},
        {1, 1, 1, 10, 1},
        {1, 1, 1, 1, 10}
    };

    double b[N] = {14, 14, 14, 14, 14};
    double x[N];
    double x_nuevo[N];

    // inicializar x en cero
    for (int i = 0; i < N; i++) {
        x[i] = 0.0;
    }

    printf("Hilos disponibles: %d\n", omp_get_max_threads());
    double inicio = omp_get_wtime();

    for (int iter = 0; iter < ITER; iter++) {

        // TODO: paralelizar con #pragma omp parallel for
        for (int i = 0; i < N; i++) {
            double suma = 0.0;

            for (int j = 0; j < N; j++) {
                if (j != i) {
                    suma += A[i][j] * x[j];
                }
            }
            x_nuevo[i] = (b[i] - suma) / A[i][i];
        }

        // verificar convergencia
        double error = 0.0;
        for (int i = 0; i < N; i++) {
            error += fabs(x_nuevo[i] - x[i]);
            x[i] = x_nuevo[i];
        }

        if (error < TOL) {
            printf("Convergio en iteracion %d\n", iter + 1);
            break;
        }
    }

    double fin = omp_get_wtime();

    printf("Solucion:\n");
    for (int i = 0; i < N; i++) {
        printf("x[%d] = %f\n", i, x[i]);
    }
    printf("Tiempo: %f segundos\n", fin - inicio);

    return 0;
}