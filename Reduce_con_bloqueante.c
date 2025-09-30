#include <mpi.h>
#include <stdio.h>

// suma de todos los datos en el root.
void my_reduce(int *send_data, int *recv_data, int root, int rank, int size) {
    if (rank == root) {
        *recv_data = *send_data; // inicia con su valor
        int tmp;
        for (int i = 0; i < size; i++) {
            if (i != root) {
                MPI_Recv(&tmp, 1, MPI_INT, i, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
                *recv_data += tmp;
            }
        }
    } else {
        MPI_Send(send_data, 1, MPI_INT, root, 0, MPI_COMM_WORLD);
    }
}

int main(int argc, char **argv) {
    MPI_Init(&argc, &argv);
    int rank, size, data, result;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    data = rank + 1; // proceso 0 manda 1, proc 1 manda 2... y asi los demás.
    my_reduce(&data, &result, 0, rank, size);

    if (rank == 0) printf("Suma total = %d\n", result);

    MPI_Finalize();
    return 0;
}
