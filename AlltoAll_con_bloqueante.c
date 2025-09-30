#include <mpi.h>
#include <stdio.h>

// cada proceso envía un dato a todos y recibe uno de todos.
void my_alltoall(int *send_data, int *recv_data, int rank, int size) {
    // cada proceso se copia a sí mismo
    recv_data[rank] = send_data[rank];
    for (int i = 0; i < size; i++) {
        if (i != rank) {
            // se envia el dato al proceso i
            MPI_Send(&send_data[i], 1, MPI_INT, i, 0, MPI_COMM_WORLD);
            // se recibe el dato del proceso i
            MPI_Recv(&recv_data[i], 1, MPI_INT, i, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        }
    }
}

int main(int argc, char **argv) {
    MPI_Init(&argc, &argv);
    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    int send_data[100], recv_data[100];
    // cada proceso prepara un vector distinto
    for (int i = 0; i < size; i++) send_data[i] = rank * 10 + i;

    my_alltoall(send_data, recv_data, rank, size);

    printf("Proceso %d recibió: ", rank);
    for (int i = 0; i < size; i++) printf("%d ", recv_data[i]);
    printf("\n");

    MPI_Finalize();
    return 0;
}
