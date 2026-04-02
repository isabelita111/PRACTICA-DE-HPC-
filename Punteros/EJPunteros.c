#include <stdio.h>

void ordenar(int *arr, int n) {
    int i, j;
    int temp;

    for (i = 0; i < n - 1; i++) {
        for (j = 0; j < n - i - 1; j++) {

            // comparar usando punteros
            if ( *(arr + j) > *(arr + j + 1) ) {

                //  intercambiar usando punteros
                temp = *(arr + j);
                 *(arr + j)= *(arr + j + 1);
                *(arr + j + 1) = temp;
            }
        }
    }
}

int main() {
    int arr[] = {5, 3, 8, 4, 1};
    int n = 5;

    ordenar(arr, n);

    // imprimir resultado
    for (int i = 0; i < n; i++) {
        printf("%d ", arr[i]);
    }

    return 0;
}