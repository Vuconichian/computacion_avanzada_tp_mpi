#include <mpi.h>
#include <stdio.h>

// el root reparte un vector, cada proceso recibe un valor distinto.
void my_scatter(int *send_data, int *recv_data, int root, int rank, int size) {
    if (rank == root) {
        // se queda con su parte
        *recv_data = send_data[root];
        // y reparte las demás
        for (int i = 0; i < size; i++) {
            if (i != root)
                MPI_Send(&send_data[i], 1, MPI_INT, i, 0, MPI_COMM_WORLD);
        }
    } else {
        MPI_Recv(recv_data, 1, MPI_INT, root, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    }
}

int main(int argc, char **argv) {
    MPI_Init(&argc, &argv);
    int rank, size, recv_data;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    int send_data[100];
    if (rank == 0) {
        for (int i = 0; i < size; i++) send_data[i] = i + 10; // Datos de ejemplo
    }

    my_scatter(send_data, &recv_data, 0, rank, size);
    printf("Proceso %d recibió %d\n", rank, recv_data);

    MPI_Finalize();
    return 0;
}
