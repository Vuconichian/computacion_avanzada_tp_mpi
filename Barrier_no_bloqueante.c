#include <mpi.h>
#include <stdio.h>

int main(int argc, char** argv) {
    int rank, size;
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    int msg = 1;
    MPI_Request reqs[size - 1];
    MPI_Status stats[size - 1];

    if (rank == 0) {
        // espera notificación de todos los procesos
        for (int i = 1; i < size; i++) {
            MPI_Irecv(&msg, 1, MPI_INT, i, 0, MPI_COMM_WORLD, &reqs[i-1]);
        }
        MPI_Waitall(size - 1, reqs, stats);

        // cuando todos avisaron, root notifica que pueden continuar
        for (int i = 1; i < size; i++) {
            MPI_Isend(&msg, 1, MPI_INT, i, 0, MPI_COMM_WORLD, &reqs[i-1]);
        }
        MPI_Waitall(size - 1, reqs, stats);

    } else {
        // cada proceso avisa al root que llegó a la barrera
        MPI_Isend(&msg, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, &reqs[0]);
        MPI_Wait(&reqs[0], MPI_STATUS_IGNORE);

        // cada proceso espera la confirmación para seguir
        MPI_Irecv(&msg, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, &reqs[0]);
        MPI_Wait(&reqs[0], MPI_STATUS_IGNORE);
    }

    printf("Proceso %d pasó la barrera\n", rank);

    MPI_Finalize();
    return 0;
}
