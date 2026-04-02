#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main() {
    int n;

    printf("Cantidad de cadenas: ");
    scanf("%d", &n);

    // reservar memoria para lista 
    char **lista = (char **)malloc(n * sizeof(char *));

    for (int i = 0; i < n; i++) {

        // buffer temporal
        char temp[100];

        printf("Cadena %d: ", i + 1);
        scanf("%s", temp);

        // reservar memoria para cada string
        lista[i] = (char *)malloc(strlen(temp) + 1);

        // copiar string
        strcpy(lista[i], temp);
    }

    printf("\nCadenas ingresadas:\n");

    for (int i = 0; i < n; i++) {
        printf("%s\n", lista[i]);
    }

    // liberar memoria
    for (int i = 0; i < n; i++) {
        // liberar cada string
        free(lista[i]);
    }

    // liberar lista
    free(lista);

    return 0;
}