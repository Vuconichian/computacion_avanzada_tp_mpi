#include <stdio.h>
#include <mpi.h>

int main(int argc, char **argv) {
    int rank, size;
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    if (rank == 0) {
        printf("Hola, soy el proceso 0 (hay %d procesos) y recibo:\n", size);
        for (int i = 1; i < size; i++) {
            char mensaje[50];
            MPI_Recv(mensaje, 50, MPI_CHAR, i, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            printf("%s\n", mensaje);
        }
    } else {
        char mensaje[50];
        sprintf(mensaje, "Hola desde el proceso %d", rank);
        // envío del mensaje al proceso 0
        MPI_Send(mensaje, 50, MPI_CHAR, 0, 0, MPI_COMM_WORLD);
    }

    MPI_Finalize();
    return 0;
}
