#include <mpi.h>
#include <stdio.h>

int main(int argc, char** argv) {
    // Root reparte un valor distinto a cada proceso
    if (rank == root) {
        for (int i = 0; i < size; i++) {
            if (i != root) {
                MPI_Isend(&sendbuf[i], 1, MPI_INT, i, 0, MPI_COMM_WORLD, &reqs[i]);
            } else {
                // recibe su propio valor directamente
                recvbuf = sendbuf[i];
            }
        }
        MPI_Waitall(size - 1, reqs, stats);
    } else {
        // Los demás reciben su parte
        MPI_Irecv(&recvbuf, 1, MPI_INT, root, 0, MPI_COMM_WORLD, &reqs[0]);
        MPI_Wait(&reqs[0], MPI_STATUS_IGNORE);
    }

    MPI_Finalize();
    return 0;
}