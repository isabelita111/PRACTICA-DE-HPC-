#include <stdio.h>
#include <math.h>
#include <omp.h>
 
/* Función a integrar  */
double f(double x) {
    return exp(-x * x);   /* f(x) = e^(-x^2) */
}
 
 
/* 
      1. SIMPSON 1/3 SIMPLE
      - h = (b - a) / 2
     - m = (a + b) / 2
      - I = (h/3) * [f(a) + 4*f(m) + f(b)]
  */
double simpson13_simple(double a, double b) {
    double h = (b - a) / 2.0;
    double m = (a + b) / 2.0;
    return (h / 3.0) * (f(a) + 4.0 * f(m) + f(b));
}
 
 
/*
      2. SIMPSON 3/8 SIMPLE
      - h = (b - a) / 3
     - I = (3h/8) * [f(x0) + 3*f(x1) + 3*f(x2) + f(x3)]
  */
double simpson38_simple(double a, double b) {
    double h  = (b - a) / 3.0;
    double x1 = a + h;
    double x2 = a + 2.0 * h;
    return (3.0 * h / 8.0) * (f(a) + 3.0 * f(x1) + 3.0 * f(x2) + f(b));
}
 
 
/* 
      3. SIMPSON 1/3 MÚLTIPLE
     - n DEBE ser par
     - h = (b - a) / n
      - coeficientes: 1, 4, 2, 4, 2, ..., 4, 1
   */
double simpson13_multiple(double a, double b, int n) {
    if (n % 2 != 0) {
        printf("ERROR: n debe ser par\n");
        return 0.0;
    }
 
    double h        = (b - a) / (double)n;
    double suma_imp = 0.0;
    double suma_par = 0.0;
 
    for (int i = 1; i <= n - 1; i += 2)
        suma_imp += f(a + i * h);
 
    for (int i = 2; i <= n - 2; i += 2)
        suma_par += f(a + i * h);
 
    return (h / 3.0) * (f(a) + 4.0 * suma_imp + 2.0 * suma_par + f(b));
}
 
 
/* 
      4. SIMPSON 3/8 MÚLTIPLE
      - n DEBE ser múltiplo de 3
      - h = (b - a) / n
     - coeficientes: 1, 3, 3, 1, 3, 3, 1, ...
 */
double simpson38_multiple(double a, double b, int n) {
    if (n % 3 != 0) {
        printf("ERROR: n debe ser múltiplo de 3\n");
        return 0.0;
    }
 
    double h    = (b - a) / (double)n;
    double suma = 0.0;
 
    for (int i = 1; i <= n - 1; i++) {
        double coef = (i % 3 == 0) ? 2.0 : 3.0;
        suma += coef * f(a + i * h);
    }
 
    return (3.0 * h / 8.0) * (f(a) + suma + f(b));
}
 
 
int main(void) {
    const double A  = -1.0;
    const double B  =  1.0;
    const int    N1 = 1000000;   /* par,           para 1/3 múltiple */
    const int    N2 = 999999;    /* múltiplo de 3, para 3/8 múltiple */
 
    double ref = sqrt(M_PI) * erf(1.0);
 
    printf("=============================================\n");
    printf("  Simpson serial   f(x)=e^(-x^2)  en [-1,1]\n");
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
 
    
    return 0;
}