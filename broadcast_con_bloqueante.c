#include <mpi.h>
#include <stdio.h>

// el root envía el mismo dato a todos los demás procesos.
void my_bcast(int *data, int root, int rank, int size) {
    if (rank == root) {
        // reparte el dato a todos
        for (int i = 0; i < size; i++) {
            if (i != root)
                MPI_Send(data, 1, MPI_INT, i, 0, MPI_COMM_WORLD);
        }
    } else {
        // Los demás lo reciben
        MPI_Recv(data, 1, MPI_INT, root, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    }
}

int main(int argc, char **argv) {
    MPI_Init(&argc, &argv);
    int rank, size, data;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    if (rank == 0) data = 42; // Root define el dato
    my_bcast(&data, 0, rank, size);

    printf("Proceso %d recibió %d\n", rank, data);

    MPI_Finalize();
    return 0;
}
