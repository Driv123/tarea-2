#include <stdio.h>
            #include <stdlib.h>
            #include <pthread.h>

                typedef struct {
                    int thread_id;
                    int cant;
                    int precio;
                    int total;
                    char* message;
                } ThreadArgs;

                void* threadFunction1(void* args) {
                    ThreadArgs* thread_args = (ThreadArgs*)args;
                    printf("Hilo %d:\nCantidad de leches: %d\nPrecio de la leche: %d\nTotal: %d\n%s\n", thread_args->thread_id, thread_args->cant, thread_args->precio, thread_args->total, thread_args->message);
                    pthread_exit(NULL); 
                }

                void* threadFunction2(void* args) {
                    ThreadArgs* thread_args = (ThreadArgs*)args;
                    printf("Hilo %d:\nCantidad de carnes: %d\nPrecio de la carne: %d\nTotal: %d\n%s\n", thread_args->thread_id, thread_args->cant, thread_args->precio, thread_args->total, thread_args->message);
                    pthread_exit(NULL); 
                }

                int main() {
                    int quant1 = 0;
                    int quant2 = 0;
                    int total1 = 0;
                    int total2 = 0;
                    int precio1 = 7;
                    int precio2 = 15;
                    int totalf = 0;

                    printf("Bienvenido al pasillo de leche\n");
                    printf("Ingrese la cantidad de leches que desea comprar, recuerde que su precio es de $7\n");
                    scanf("%d", &quant1);
                    total1 = precio1*quant1;

                    ThreadArgs args1 = {1, quant1, precio1, total1, "Compra desde el pasillo de leches"};

                    printf("Bienvenido al pasillo de carnes\n");
                    printf("Ingrese la cantidad de carnes que desea comprar, recuerde que su precio es de $15\n");
                    scanf("%d", &quant2);
                    total2 = precio2*quant2;

                    ThreadArgs args2 = {2, quant2, precio2, total2, "Compra desde el pasillo de carne"};

                
                    pthread_t thread1, thread2;
                    if (pthread_create(&thread1, NULL, threadFunction1, (void*)&args1) != 0) {
                        perror("Error al crear el primer hilo"); 
                        exit(EXIT_FAILURE);
                    }

                    if (pthread_create(&thread2, NULL, threadFunction2, (void*)&args2) != 0) {
                        perror("Error al crear el segundo hilo"); 
                        exit(EXIT_FAILURE);
                    }

                   
                    if (pthread_join(thread1, NULL) != 0) {
                        perror("Error al unir el primer hilo"); 
                        exit(EXIT_FAILURE);
                    }

                    if (pthread_join(thread2, NULL) != 0) {
                        perror("Error al unir el segundo hilo"); 
                        exit(EXIT_FAILURE);
                    }

                    totalf = total1+total2;
                    printf("Hilo principal: El total de su compra final es %d.\n", totalf);

                    return 0;
                }