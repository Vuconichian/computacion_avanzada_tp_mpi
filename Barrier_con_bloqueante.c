#include <mpi.h>
#include <stdio.h>

// el proceso 0 controla la sincronización: primero notifica a todos y espera confirmaciones antes de continuar.
void my_barrier(int rank, int size) {
    int token = 1;
    if (rank == 0) {
        // envía señal a todos
        for (int i = 1; i < size; i++)
            MPI_Send(&token, 1, MPI_INT, i, 0, MPI_COMM_WORLD);

        // espera las confirmaciones de todos
        for (int i = 1; i < size; i++)
            MPI_Recv(&token, 1, MPI_INT, i, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    } else {
        // los procesos reciben la señal y responden
        MPI_Recv(&token, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        MPI_Send(&token, 1, MPI_INT, 0, 0, MPI_COMM_WORLD);
    }
}

int main(int argc, char **argv) {
    MPI_Init(&argc, &argv);
    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    printf("Proceso %d antes de barrier\n", rank);
    my_barrier(rank, size);
    printf("Proceso %d después de barrier\n", rank);

    MPI_Finalize();
    return 0;
}
