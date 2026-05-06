#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <omp.h>
 
#define L      1.0
#define ALPHA  0.01
#define T_MAX  1.0
#define NT     5000
#define NX     100
 
double condicion_inicial(double x) {
    return sin(M_PI * x / L);
}
 
double solucion_exacta(double x, double t) {
    return sin(M_PI * x / L) * exp(-ALPHA * pow(M_PI, 2) / (L * L) * t);
}
 
int main(void) {
 
    double dx = L / (NX - 1);
    double dt = T_MAX / NT;
    double r  = ALPHA * dt / (dx * dx);
 
    printf("r = %.4f  (debe ser <= 0.5)\n", r);
    printf("Hilos OpenMP: %d\n", omp_get_max_threads());
 
    double *u       = (double *)malloc(NX * sizeof(double));
    double *u_nueva = (double *)malloc(NX * sizeof(double));
 
    for (int i = 0; i < NX; i++) {
        double x = i * dx;
        u[i] = condicion_inicial(x);
    }
    u[0]      = 0.0;
    u[NX - 1] = 0.0;
 
    FILE *fp = fopen("calor_paralelo.csv", "w");
    fprintf(fp, "t,x,u_num,u_exact\n");
 
    for (int i = 0; i < NX; i++) {
        double x = i * dx;
        fprintf(fp, "%.6f,%.6f,%.10f,%.10f\n",
                0.0, x, u[i], solucion_exacta(x, 0.0));
    }
 
    double t_inicio = omp_get_wtime();
 
    for (int n = 0; n < NT; n++) {
 
        #pragma omp parallel for
        for (int i = 1; i < NX - 1; i++) {
            u_nueva[i] = u[i] + r * (u[i+1] - 2*u[i] + u[i-1]);
        }
 
        u_nueva[0]      = 0.0;
        u_nueva[NX - 1] = 0.0;
 
        #pragma omp parallel for
        for (int i = 0; i < NX; i++)
            u[i] = u_nueva[i];
 
        if (n % 50 == 0) {
            double t_actual = (n + 1) * dt;
            for (int i = 0; i < NX; i++) {
                double x = i * dx;
                fprintf(fp, "%.6f,%.6f,%.10f,%.10f\n",
                        t_actual, x, u[i], solucion_exacta(x, t_actual));
            }
        }
    }
 
    double t_fin = omp_get_wtime();
 
    fclose(fp);
    free(u);
    free(u_nueva);
 
    printf("Tiempo paralelo: %.6f s\n", t_fin - t_inicio);
    printf("Speedup = t_serial / t_paralelo\n");
 
    return 0;
}