#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h> // para strlen

int main(int argc, char **argv) {
    MPI_Init(&argc, &argv);

    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    if (argc < 2) {
        if (rank == 0) printf("Uso: mpirun -np <num_procesos> %s <vueltas>\n", argv[0]);
        MPI_Finalize();
        return 1;
    }

    int n = atoi(argv[1]); // cantidad de vueltas
    char msg[10];           // mensaje a enviar
    strcpy(msg, "Hola");    // copiamos "Hola" al mensaje

    int next = (rank + 1) % size;
    int prev = (rank - 1 + size) % size;

    for (int vuelta = 0; vuelta < n; vuelta++) {
        if (rank == 0) {
            // Proceso 0 inicia el mensaje solo en la primera vuelta
            if (vuelta == 0) {
                MPI_Send(msg, strlen(msg)+1, MPI_CHAR, next, 0, MPI_COMM_WORLD);
                printf("Proceso %d envía mensaje '%s' a proceso %d (vuelta %d)\n", rank, msg, next, vuelta+1);
            }

            MPI_Recv(msg, 10, MPI_CHAR, prev, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            printf("Proceso %d recibe mensaje '%s' del proceso %d (vuelta %d)\n", rank, msg, prev, vuelta+1);

            if (vuelta < n-1) {
                MPI_Send(msg, strlen(msg)+1, MPI_CHAR, next, 0, MPI_COMM_WORLD);
                printf("Proceso %d envía mensaje '%s' a proceso %d (vuelta %d)\n", rank, msg, next, vuelta+2);
            }
        } else {
            MPI_Recv(msg, 10, MPI_CHAR, prev, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            printf("Proceso %d recibe mensaje '%s' del proceso %d (vuelta %d)\n", rank, msg, prev, vuelta+1);

            MPI_Send(msg, strlen(msg)+1, MPI_CHAR, next, 0, MPI_COMM_WORLD);
            printf("Proceso %d envía mensaje '%s' a proceso %d (vuelta %d)\n", rank, msg, next, vuelta+1);
        }
    }

    MPI_Finalize();
    return 0;
}