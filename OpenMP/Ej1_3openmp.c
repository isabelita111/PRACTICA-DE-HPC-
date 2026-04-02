#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

#define PASOS 20
#define DT    1.0

int main() {

    double pos[PASOS];
    double vel[PASOS];

    // condiciones iniciales
    pos[0] = 0.0;
    vel[0] = 5.0;
    double a = 2.0;

    double inicio = omp_get_wtime();

    #pragma omp parallel
    {
        #pragma omp single
        {
            for (int t = 1; t < PASOS; t++) {

                #pragma omp task depend(in: vel[t-1], pos[t-1]) depend(out: vel[t], pos[t])
                {
                    vel[t] = vel[t-1] + a * DT;
                    pos[t] = pos[t-1] + vel[t] * DT;
                }

            }
        }
    }

    double fin = omp_get_wtime();

    for (int t = 0; t < PASOS; t++) {
        printf("t=%2d | pos=%8.2f m | vel=%6.2f m/s\n", t, pos[t], vel[t]);
    }
    printf("Tiempo: %f segundos\n", fin - inicio);

    return 0;
}