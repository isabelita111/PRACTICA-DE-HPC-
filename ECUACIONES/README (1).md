
---

## Ejercicio 1: Integración Numérica — Reglas de Simpson

### ¿Qué se hizo?

Se calculó el área bajo la curva de la función:

$$f(x) = e^{-x^2}$$

en el intervalo $[-1, 1]$. El valor real de esa área es aproximadamente **1.4936482656**, y se usaron cuatro métodos numéricos para aproximarlo.

### ¿Qué métodos se usaron?

Todos los métodos de Simpson funcionan dividiendo el área en pedazos pequeños y aproximando cada pedazo con una curva. La diferencia entre ellos está en cuántos puntos usan por pedazo y cómo los combinan:

| Método | Cómo funciona | Condición |
|---|---|---|
| Simpson 1/3 simple | Usa 3 puntos y una parábola | Sin condición |
| Simpson 3/8 simple | Usa 4 puntos y una curva cúbica | Sin condición |
| Simpson 1/3 múltiple | Repite el 1/3 en muchos pedazos | El número de pedazos debe ser par |
| Simpson 3/8 múltiple | Repite el 3/8 en muchos pedazos | El número de pedazos debe ser múltiplo de 3 |

### Fórmulas

**Simpson 1/3 simple:**
$$I \approx \frac{h}{3}\left[f(a) + 4f(m) + f(b)\right], \quad h = \frac{b-a}{2}$$

**Simpson 3/8 simple:**
$$I \approx \frac{3h}{8}\left[f(x_0) + 3f(x_1) + 3f(x_2) + f(x_3)\right], \quad h = \frac{b-a}{3}$$

**Simpson 1/3 múltiple:**
$$I \approx \frac{h}{3}\left[f(a) + 4\sum_{i \text{ impar}} f(x_i) + 2\sum_{i \text{ par}} f(x_i) + f(b)\right]$$

**Simpson 3/8 múltiple:**
$$I \approx \frac{3h}{8}\left[f(a) + \sum_{i=1}^{n-1} c_i \cdot f(x_i) + f(b)\right], \quad c_i = \begin{cases} 3 & i \not\equiv 0 \pmod{3} \\ 2 & i \equiv 0 \pmod{3} \end{cases}$$

### ¿Cómo se paralelizó?

Los métodos simples solo hacen 3 o 4 operaciones, así que no tiene sentido repartirlas entre varios procesadores — es tan poco trabajo que no vale la pena.

Los métodos múltiples sí hacen millones de operaciones, y como cada una es independiente de las demás, se pueden repartir fácilmente entre varios procesadores usando OpenMP:

```c
#pragma omp parallel for reduction(+:suma_imp)
for (int i = 1; i <= n - 1; i += 2)
    suma_imp += f(a + i * h);
```

La instrucción `reduction` se usa para que cada procesador acumule su parte del resultado sin pisarse con los demás.

### Resultados

Se usó n = 1,000,000 pedazos para Simpson 1/3 y n = 999,999 para Simpson 3/8, con 4 procesadores en paralelo.

| Método | Resultado | Error | Tiempo serial | Tiempo paralelo | Speedup |
|---|---|---|---|---|---|
| Simpson 1/3 simple   | 1.5785862941 | 8.49e-02 | 0.000000 s | 0.001000 s | — |
| Simpson 3/8 simple   | 1.5261986958 | 3.26e-02 | 0.000000 s | 0.001000 s | — |
| Simpson 1/3 múltiple | 1.4936482656 | 3.55e-14 | 0.010000 s | 0.004000 s | 2.5x |
| Simpson 3/8 múltiple | 1.4936482656 | 2.71e-14 | 0.012000 s | 0.004000 s | 3.0x |

---

## Ejercicio 2: Ecuación de Calor 1D

### ¿Qué se hizo?

Se simuló cómo se distribuye el calor a lo largo de una barra metálica con el tiempo. La barra empieza caliente en el centro y fría en los extremos, y se observa cómo el calor se va disipando.

La ecuación que describe este fenómeno es:

$$\frac{\partial u(x,t)}{\partial t} = \alpha \frac{\partial^2 u(x,t)}{\partial x^2}$$

**Parámetros usados:**
- Longitud de la barra: L = 1.0
- Velocidad con que conduce el calor: α = 0.01
- Temperatura inicial: forma de ola sinusoidal — $u(x, 0) = \sin\left(\frac{\pi x}{L}\right)$
- Extremos siempre a temperatura 0: $u(0, t) = u(L, t) = 0$
- Tiempo total de simulación: T = 1.0

La solución exacta con la que se compararon los resultados es:

$$u(x,t) = \sin\left(\frac{\pi x}{L}\right) e^{-\frac{\alpha \pi^2}{L^2} t}$$

### ¿Cómo se resolvió?

Se usó el método de Diferencias Finitas Explícito (FTCS), que básicamente divide la barra en 100 puntos y avanza el tiempo en 5000 pasos pequeños. En cada paso, la temperatura de cada punto se actualiza usando la temperatura de sus vecinos:

$$u_i^{n+1} = u_i^n + r\left(u_{i+1}^n - 2u_i^n + u_{i-1}^n\right), \quad r = \frac{\alpha \cdot \Delta t}{\Delta x^2}$$

El valor r = 0.0196 indica que el método es estable (debe ser menor a 0.5).

### ¿Cómo se paralelizó?

En cada paso de tiempo, el cálculo de cada punto de la barra es independiente del resto, así que se pueden calcular todos al mismo tiempo con OpenMP:

```c
#pragma omp parallel for
for (int i = 1; i < NX - 1; i++) {
    u_nueva[i] = u[i] + r * (u[i+1] - 2*u[i] + u[i-1]);
}
```

### Resultados

| Medición | Valor |
|---|---|
| Error promedio respecto a la solución exacta | 4.14e-06 |
| Tiempo serial | 0.023000 s |
| Tiempo paralelo (4 procesadores) | 0.658000 s |
| Speedup | 0.035x |

### Gráficas generadas

**Comparación numérica vs exacta:** muestra que la solución del programa y la solución matemática real son prácticamente iguales al final de la simulación.

**Evolución temporal:** muestra cómo la temperatura en la barra baja con el tiempo — las curvas azules (tiempo inicial) son más altas que las amarillas (tiempo final), reflejando la disipación del calor.

**Superficie 3D:** muestra todo el proceso en una sola imagen — la posición en la barra, el tiempo, y la temperatura al mismo tiempo.

---

## Conclusiones

1. **Los métodos múltiples de Simpson sí fueron más rápidos en paralelo.** Con 4 procesadores, Simpson 1/3 múltiple fue 2.5 veces más rápido y Simpson 3/8 múltiple fue 3 veces más rápido que en serial. Esto funciona bien porque hay un millón de operaciones que repartir entre los procesadores.

2. **Los métodos simples de Simpson no se beneficiaron del paralelo.** Son tan pocas operaciones (3 o 4) que el tiempo que se gasta en organizar los procesadores es mayor que el tiempo del cálculo en sí.

3. **En la ecuación de calor, el serial fue más rápido que el paralelo.** La barra tiene solo 100 puntos, así que en cada paso de tiempo hay muy poco trabajo que repartir. El costo de coordinar 4 procesadores en cada uno de los 5,000 pasos terminó siendo mucho mayor que simplemente calcularlo todo en uno solo.

4. **Paralelizar no siempre es mejor.** Cuando el problema es pequeño, organizar los procesadores cuesta más tiempo del que se ahorra. La paralelización funciona bien cuando hay muchísimo trabajo que hacer, como en los métodos múltiples de Simpson.

5. **Los resultados numéricos fueron los mismos en serial y paralelo.** Dividir el trabajo entre procesadores no afectó la precisión de los cálculos en ningún caso.


