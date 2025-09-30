#include <mpi.h>
#include <stdio.h>

int main(int argc, char** argv) {
// Cada proceso envía su valor al root
    if (rank == root) {
        recvbuf[root] = sendbuf; // root guarda su propio valor
        for (int i = 0; i < size; i++) {
            if (i != root) {
                MPI_Irecv(&recvbuf[i], 1, MPI_INT, i, 0, MPI_COMM_WORLD, &reqs[i]);
            }
        }
        MPI_Waitall(size - 1, reqs, stats);
    } else {
        MPI_Isend(&sendbuf, 1, MPI_INT, root, 0, MPI_COMM_WORLD, &reqs[0]);
        MPI_Wait(&reqs[0], MPI_STATUS_IGNORE);
    }

    MPI_Finalize();
    return 0;
}