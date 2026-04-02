#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

#define PUNTOS 100000000  // 100 millones de puntos aleatorios

int main() {

    long long dentro = 0;  // contador de puntos dentro del circulo

    printf("Hilos disponibles: %d\n", omp_get_max_threads());
    double inicio = omp_get_wtime();

    // paralelizar el conteo de puntos con reduccion para evitar condicion de carrera
    #pragma omp parallel for reduction(+:dentro)
    for (long long i = 0; i < PUNTOS; i++) {

        // generar punto aleatorio (x, y) entre 0 y 1
        double x = rand() / (double)RAND_MAX;
        double y = rand() / (double)RAND_MAX;

        // verificar si el punto cae dentro del circulo unitario
        if (x * x + y * y <= 1.0) {
            dentro++;  // acumular de forma segura gracias al reduction
        }
    }

    double fin = omp_get_wtime();

    // pi ≈ 4 * (puntos dentro / total de puntos)
    double pi = 4.0 * dentro / PUNTOS;
    printf("Pi aproximado: %f\n", pi);
    printf("Tiempo: %f segundos\n", fin - inicio);

    return 0;
}