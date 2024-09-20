#include <stdio.h>
#include <pthread.h>

#define n 13
#define m 13


void obtener_menor(int matriz[n][m], int menor[n][m], int fila_omitida, int columna_omitida, int tam) {
    int i = 0, j = 0;

    for (int fila = 0; fila < tam; fila++) {
        if (fila == fila_omitida)
            continue;
        j = 0;
        for (int columna = 0; columna < tam; columna++) {
            if (columna == columna_omitida)
                continue;
            menor[i][j] = matriz[fila][columna];
            j++;
        }
        i++;
    }
}

int determinante(int matriz[n][m], int tam) {
    if (tam == 1)
        return matriz[0][0];
    if (tam == 2)
        return matriz[0][0] * matriz[1][1] - matriz[0][1] * matriz[1][0];

    int det = 0;
    int menor[n][n];
    for (int col = 0; col < tam; col++) {
        obtener_menor(matriz, menor, 0, col, tam);
        int signo = (col % 2 == 0) ? 1 : -1;
        det += signo * matriz[0][col] * determinante(menor, tam - 1);
    }

    return det;
}


void *philo(void *arg) {
    int (*vec)[n] = (int (*)[n])arg;
    int det = determinante(vec, n);  
    printf("Determinante: %d\n", det);
    return NULL;
}

int main() {
    int i, j;
    int vec[n][m];
    pthread_t thread;

    for (i = 0; i < n; i++) {
        for (j = 0; j < m; j++) {
            printf("Ingrese el término [%d][%d]: ", i, j);
            scanf("%d", &vec[i][j]);
        }
    }

    pthread_create(&thread, NULL, philo, (void *)vec);


    pthread_join(thread, NULL);

    return 0;
}
