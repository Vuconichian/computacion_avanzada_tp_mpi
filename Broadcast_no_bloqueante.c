#include <mpi.h>
#include <stdio.h>

int main(int argc, char** argv) {
    // el root envía el valor a todos, los demás lo reciben
    if (rank == root) {
        for (int i = 0; i < size; i++) {
            if (i != root) {
                MPI_Isend(&data, 1, MPI_INT, i, 0, MPI_COMM_WORLD, &reqs[i]);
            }
        }
        MPI_Waitall(size - 1, reqs, stats);
    } else {
        MPI_Irecv(&data, 1, MPI_INT, root, 0, MPI_COMM_WORLD, &reqs[0]);
        MPI_Wait(&reqs[0], MPI_STATUS_IGNORE);
    }

    MPI_Finalize();
    return 0;
}
