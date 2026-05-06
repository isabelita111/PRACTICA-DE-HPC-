#include <stdio.h>
#include <math.h>
#include <omp.h>
 
/*  Función a integrar  */
double f(double x) {
    return exp(- x * x);   /* f(x) = e^(-x^2) */
}
 
 
/* 
     1. SIMPSON 1/3 SIMPLE  (paralela)
      Solo 3 puntos → no hay bucle que paralelizar.
     Se usa #pragma omp parallel + single para mantener
        la estructura paralela.
 */
double simpson13_simple(double a, double b) {
    double h  = (b - a) / 2.0;
    double m  = (a + b) / 2.0;
    double resultado = 0.0;
 
    #pragma omp parallel
    {
        #pragma omp single
        resultado = (h / 3.0) * (f(a) + 4.0 * f(m) + f(b));
    }
 
    return resultado;
}
 
 
/* 
      2. SIMPSON 3/8 SIMPLE  (paralela)
      4 puntos → tampoco hay bucle.
  */
double simpson38_simple(double a, double b) {
    double h  = (b - a) / 3.0;
    double x1 = a + h;
    double x2 = a + 2.0 * h;
    double resultado = 0.0;
 
    #pragma omp parallel
    {
        #pragma omp single
        resultado = (3.0 * h / 8.0) * (f(a) + 3.0 * f(x1) + 3.0 * f(x2) + f(b));
    }
 
    return resultado;
}
 
 
/* 
     3. SIMPSON 1/3 MÚLTIPLE  (paralela)
     Aquí sí hay speedup real.
     #pragma omp parallel for reduction(+:variable)
     hace que cada hilo acumule su parte y al final
     OpenMP suma todo sin condición de carrera.
  */
double simpson13_multiple(double a, double b, int n) {
    if (n % 2 != 0) {
        printf("ERROR: n debe ser par\n");
        return 0.0;
    }
 
    double h        = (b - a) / (double)n;
    double suma_imp = 0.0;
    double suma_par = 0.0;
 
    #pragma omp parallel for reduction(+:suma_imp)
    for (int i = 1; i <= n - 1; i += 2)
        suma_imp += f(a + i * h);
 
    #pragma omp parallel for reduction(+:suma_par)
    for (int i = 2; i <= n - 2; i += 2)
        suma_par += f(a + i * h);
 
    return (h / 3.0) * (f(a) + 4.0 * suma_imp + 2.0 * suma_par + f(b));
}
 
 
/*
     4. SIMPSON 3/8 MÚLTIPLE  (paralela)
     Un solo bucle con coeficientes variables.
*/
double simpson38_multiple(double a, double b, int n) {
    if (n % 3 != 0) {
        printf("ERROR: n debe ser múltiplo de 3\n");
        return 0.0;
    }
 
    double h    = (b - a) / (double)n;
    double suma = 0.0;
 
    #pragma omp parallel for reduction(+:suma)
    for (int i = 1; i <= n - 1; i++) {
        double coef = (i % 3 == 0) ? 2.0 : 3.0;
        suma += coef * f(a + i * h);
    }
 
    return (3.0 * h / 8.0) * (f(a) + suma + f(b));
}
 

int main(void) {
    const double A  = -1.0;
    const double B  =  1.0;
    const int    N1 = 1000000;
    const int    N2 = 999999;
 
    double ref = sqrt(M_PI) * erf(1.0);
 
    printf("=============================================\n");
    printf("  Simpson paralelo  f(x)=e^(-x^2)  en [-1,1]\n");
    printf("  Hilos: %d\n", omp_get_max_threads());
    printf("  Referencia: %.10f\n", ref);
    printf("=============================================\n\n");
 
    double resultado, t_inicio, t_fin;
 
    /* ── 1. Simpson 1/3 simple ── */
    t_inicio  = omp_get_wtime();
    resultado = simpson13_simple(A, B);
    t_fin     = omp_get_wtime();
    printf("Simpson 1/3 simple   : %.10f  (error: %.2e)  tiempo: %.6f s\n",
           resultado, fabs(resultado - ref), t_fin - t_inicio);
 
    /* ── 2. Simpson 3/8 simple ── */
    t_inicio  = omp_get_wtime();
    resultado = simpson38_simple(A, B);
    t_fin     = omp_get_wtime();
    printf("Simpson 3/8 simple   : %.10f  (error: %.2e)  tiempo: %.6f s\n",
           resultado, fabs(resultado - ref), t_fin - t_inicio);
 
    /* ── 3. Simpson 1/3 múltiple ── */
    t_inicio  = omp_get_wtime();
    resultado = simpson13_multiple(A, B, N1);
    t_fin     = omp_get_wtime();
    printf("Simpson 1/3 multiple : %.10f  (error: %.2e)  tiempo: %.6f s\n",
           resultado, fabs(resultado - ref), t_fin - t_inicio);
 
    /* ── 4. Simpson 3/8 múltiple ── */
    t_inicio  = omp_get_wtime();
    resultado = simpson38_multiple(A, B, N2);
    t_fin     = omp_get_wtime();
    printf("Simpson 3/8 multiple : %.10f  (error: %.2e)  tiempo: %.6f s\n",
           resultado, fabs(resultado - ref), t_fin - t_inicio);
 
    printf("\n  Speedup = t_serial / t_paralelo\n");
 
    return 0;
}
 