#include <stdio.h>
#include <mpi.h>

int main(int argc, char **argv) {
    int rank, size;
    int coordinador = 2; // Cambiá este valor para elegir otro coordinador

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    if (rank == coordinador) {
        printf("Hola, soy el proceso %d (hay %d procesos) y recibo:\n", coordinador, size);
        for (int i = 0; i < size; i++) {
            if (i != coordinador) {
                char mensaje[50];
                MPI_Recv(mensaje, 50, MPI_CHAR, i, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
                printf("%s\n", mensaje);
            }
        }
    } else {
        char mensaje[50];
        sprintf(mensaje, "Hola desde el proceso %d", rank);
        MPI_Send(mensaje, 50, MPI_CHAR, coordinador, 0, MPI_COMM_WORLD);
    }

    MPI_Finalize();
    return 0;
}
