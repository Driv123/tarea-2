#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>  


typedef struct {
    int *Numeros_Cuenta;
    float *Dinero_Cuenta;
    int Cuenta;
    float Cantidad;
} datos_deposito;

typedef struct {
    int *Numeros_Cuenta;
    float *Dinero_Cuenta;
    int Cuenta;
} datos_consulta;


int encontrar_Usuario(int*, int);
void registrar_Usuario(int*, float*, int);
void *depositar_Usuario(void *args);
void *consultar_Saldo(void *args);

int main() {
    int Numeros_Cuenta[10];
    float Dinero_Cuenta[10];
    int Opcion = 0, Salida = 0;
    int Usuarios = 0;

    pthread_t hilo_deposito;
	pthread_t hilo_consulta;

    printf("\n\n\t\t\tBienvenido al cajero automatico en fase beta que usa hilos xd\n\n\n");

    do {
        printf("\nSeleccione una de las siguientes opciones: \n\n");
        printf("1. Registrar usuario\n");
        printf("2. Realizar deposito\n");
        printf("3. Consultar saldo\n");
        printf("4. Salir\n");
        printf("Ingrese una opcion: ");
        scanf("%d", &Opcion);

        switch (Opcion) {
            case 1:
                if (Usuarios <= 9) {
                    registrar_Usuario(Numeros_Cuenta, Dinero_Cuenta, Usuarios);
                    Usuarios++;
                } else {
                    printf("\nSe ha alcanzado el numero maximo de usuarios.\n\n");
                }
                Salida = 1;
                break;
            case 2:
                if (Usuarios > 0) {
                    int Cuenta;
                    float Cantidad;
                    printf("\nIngrese el numero de cuenta: ");
                    scanf("%d", &Cuenta);
                    printf("\nIngrese la cantidad a depositar: ");
                    scanf("%f", &Cantidad);

                    
                    datos_deposito *args_dep = (datos_deposito *)malloc(sizeof(datos_deposito));
                    args_dep->Numeros_Cuenta = Numeros_Cuenta;
                    args_dep->Dinero_Cuenta = Dinero_Cuenta;
                    args_dep->Cuenta = Cuenta;
                    args_dep->Cantidad = Cantidad;

                    
                    pthread_create(&hilo_deposito, NULL, depositar_Usuario, (void *)args_dep);
                    pthread_join(hilo_deposito, NULL);

                    
                    free(args_dep);
                } else {
                    printf("\nNo hay usuarios registrados.\n\n");
                }
                Salida = 1;
                break;
            case 3:
                if (Usuarios > 0) {
                    int Cuenta;
                    printf("\nIngrese el numero de cuenta: ");
                    scanf("%d", &Cuenta);

                    
                    datos_consulta *args_con = (datos_consulta *)malloc(sizeof(datos_consulta));
                    args_con->Numeros_Cuenta = Numeros_Cuenta;
                    args_con->Dinero_Cuenta = Dinero_Cuenta;
                    args_con->Cuenta = Cuenta;

                    
                    pthread_create(&hilo_consulta, NULL, consultar_Saldo, (void *)args_con);
                    pthread_join(hilo_consulta, NULL);

                    
                    free(args_con);
                } else {
                    printf("\nNo hay usuarios registrados.\n\n");
                }
                Salida = 1;
                break;
            case 4:
                Salida = 2;
                break;
            default:
                printf("Opción invalida.\nIngrese una opción valida.\n\n");
                Salida = 1;
        }
    } while (Salida == 1);

    printf("\nGracias por utilizar el cajero automatico. Ya termine el semestre c:\n");
    return 0;
}


int encontrar_Usuario(int *Numeros_Cuenta, int Cuenta) {
	int i=0;
    for ( i = 0; i <= 9; i++) {
        if (Numeros_Cuenta[i] == Cuenta) {
            return i;
        }
    }
    return 100;
}

void registrar_Usuario(int *Numeros_Cuenta, float *Dinero_Cuenta, int Posicion) {
    printf("\nIngrese el numero de cuenta del usuario %d: ", Posicion + 1);
    scanf("%d", &Numeros_Cuenta[Posicion]);
    Dinero_Cuenta[Posicion] = 0.0;
    printf("\nUsuario registrado exitosamente.\n\n");
}


void *depositar_Usuario(void *args) {
    datos_deposito *datos = (datos_deposito *)args;
    int Posicion_Usuario = encontrar_Usuario(datos->Numeros_Cuenta, datos->Cuenta);
    
    if (Posicion_Usuario != 100) {
        datos->Dinero_Cuenta[Posicion_Usuario] += datos->Cantidad;
        printf("\nDeposito exitoso, el saldo actualizado de la cuenta es: $%.2f MXN\n\n", datos->Dinero_Cuenta[Posicion_Usuario]);
    } else {
        printf("\nNumero de cuenta no encontrado.\n\n");
    }

    return NULL;
}


void *consultar_Saldo(void *args) {
    datos_consulta *datos = (datos_consulta *)args;
    int Posicion_Usuario = encontrar_Usuario(datos->Numeros_Cuenta, datos->Cuenta);

    if (Posicion_Usuario != 100) {
        printf("\nEl saldo de la cuenta %d es: $%.2f MXN\n\n", datos->Cuenta, datos->Dinero_Cuenta[Posicion_Usuario]);
    } else {
        printf("\nNumero de cuenta no encontrado.\n\n");
    }

    return NULL;
}