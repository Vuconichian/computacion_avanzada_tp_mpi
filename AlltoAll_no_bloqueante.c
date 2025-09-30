#include <mpi.h>
#include <stdio.h>

int main(int argc, char** argv) {
// Cada proceso envía a todos y recibe de todos
    for (int i = 0; i < size; i++) {
        if (i != rank) {
            MPI_Isend(&sendbuf[i], 1, MPI_INT, i, 0, MPI_COMM_WORLD, &reqs[i]);
            MPI_Irecv(&recvbuf[i], 1, MPI_INT, i, 0, MPI_COMM_WORLD, &reqs[size+i]);
        } else {
            // Auto-copia de su propio valor
            recvbuf[i] = sendbuf[i];
        }
    }
    // Esperamos que todas las operaciones terminen
    MPI_Waitall(2*(size-1), reqs, stats);

    MPI_Finalize();
    return 0;
}