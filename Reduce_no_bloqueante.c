#include <mpi.h>
#include <stdio.h>

int main(int argc, char** argv) {
    int value = rank + 1; 
    int total, tmp;

    if (rank == 0) {
        total = value; // root comienza con su valor
        for (int i = 1; i < size; i++) {
            MPI_Irecv(&tmp, 1, MPI_INT, i, 0, MPI_COMM_WORLD, &reqs[i-1]);
        }
        MPI_Waitall(size - 1, reqs, stats);

        // Sumo todos los valores recibidos
        for (int i = 0; i < size - 1; i++) {
            total += tmp;
        }
        printf("Suma total = %d\n", total);
    } else {
        MPI_Isend(&value, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, &reqs[0]);
        MPI_Wait(&reqs[0], MPI_STATUS_IGNORE);
    }
    
    MPI_Finalize();
    return 0;
}