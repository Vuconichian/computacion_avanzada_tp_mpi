#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>

// todos mandan un dato al root.
void my_gather(int *send_data, int *recv_data, int root, int rank, int size) {
    if (rank == root) {
        // guarda su propio dato
        recv_data[root] = *send_data;
        // y recibe de los demás
        for (int i = 0; i < size; i++) {
            if (i != root) {
                MPI_Recv(&recv_data[i], 1, MPI_INT, i, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            }
        }
    } else {
        MPI_Send(send_data, 1, MPI_INT, root, 0, MPI_COMM_WORLD);
    }
}

int main(int argc, char **argv) {
    MPI_Init(&argc, &argv);
    int rank, size, data;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    data = rank + 1; // Cada proceso genera un número diferente
    int *gathered = NULL;
    if (rank == 0) gathered = malloc(size * sizeof(int));

    my_gather(&data, gathered, 0, rank, size);

    if (rank == 0) {
        printf("Root juntó: ");
        for (int i = 0; i < size; i++) printf("%d ", gathered[i]);
        printf("\n");
        free(gathered);
    }

    MPI_Finalize();
    return 0;
}
