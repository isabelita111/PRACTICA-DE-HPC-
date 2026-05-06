#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <omp.h>

/* ── Parámetros del problema 
#define L      1.0       /* longitud de la barra              */
#define ALPHA  0.01      /* difusividad térmica               */
#define T_MAX  1.0       /* tiempo máximo de simulación       */
#define NX     100       /* número de puntos espaciales       */
#define NT     5000       /* número de pasos de tiempo         */

/* ── Condición inicial */
double condicion_inicial(double x) {
    return sin(M_PI * x / L);
}

/* ── Solución analítica (para comparar)  */
double solucion_exacta(double x, double t) {
    return sin(M_PI * x / L) * exp(-ALPHA * pow(M_PI, 2) / pow(L, 2) * t);
}

int main(void) {

    /* ── Paso espacial y temporal  */
    double dx = L / (NX - 1);   
    double dt = T_MAX / NT;          

    /* ── Número de difusión (debe ser <= 0.5 para estabilidad) ── */
    double r = ALPHA * dt / (dx * dx);
    printf("r = %.4f  (debe ser <= 0.5)\n", r);
    if (r > 0.5) {
        printf("ADVERTENCIA: esquema inestable, reduce dt o aumenta dx\n");
    }

    /* ── Reservar memoria para u[NX] y u_nueva[NX] ─────── */
    double *u      = (double *)malloc(NX * sizeof(double));
    double *u_nueva = (double *)malloc(NX * sizeof(double));

    /* ── Condición inicial  */
    for (int i = 0; i < NX; i++) {
        double x = i * dx;
        u[i] = condicion_inicial(x);
    }
    /* Condiciones de contorno: extremos en 0 */
    u[0]      = 0.0;
    u[NX - 1] = 0.0;

    /* ── Abrir archivo CSV para guardar resultados  */
    FILE *fp = fopen("calor_serial.csv", "w");
    fprintf(fp, "t,x,u_num,u_exact\n");

    /* Guardar el estado inicial (t=0) */
    for (int i = 0; i < NX; i++) {
        double x = i * dx;
        fprintf(fp, "%.6f,%.6f,%.10f,%.10f\n",
                0.0, x, u[i], solucion_exacta(x, 0.0));
    }

    /* ── Medir tiempo ─ */
    double t_inicio = omp_get_wtime();

    /* ── Bucle temporal   */
    for (int n = 0; n < NT; n++) {
        for (int i = 1; i < NX - 1; i++) {
            u_nueva[i] = u[i] + r * (u[i+1] - 2*u[i] + u[i-1]);
        }

        /* Condiciones de contorno */
        u_nueva[0]      = 0.0;
        u_nueva[NX - 1] = 0.0;

        /* Copiar u_nueva en u para el siguiente paso */
        for (int i = 0; i < NX; i++)
            u[i] = u_nueva[i];

        /* Guardar cada 50 pasos para no llenar el CSV */
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

    printf("Tiempo serial: %.6f s\n", t_fin - t_inicio);
    printf("Datos guardados en calor_serial.csv\n");

    return 0;
}