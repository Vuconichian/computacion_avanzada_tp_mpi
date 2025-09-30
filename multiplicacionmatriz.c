#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char **argv) {
    int rank, size;
    int n;  // tamaño de la matriz (n x n)

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    if (rank == 0) {
        printf("Ingrese el tamaño de la matriz (n x n): ");
        fflush(stdout);
        scanf("%d", &n);
    }

    // difundir el tamaño de la matriz a todos los procesos
    MPI_Bcast(&n, 1, MPI_INT, 0, MPI_COMM_WORLD);

    if (size != n) {
        if (rank == 0)
            printf("Error: debe ejecutar el programa con %d procesos.\n", n);
        MPI_Finalize();
        return 1;
    }

    // cada proceso recibe una fila
    int *row = (int*)malloc(n * sizeof(int));
    int *vector = (int*)malloc(n * sizeof(int));
    int result;

    if (rank == 0) {
        // genera matriz y vector
        int *matrix = (int*)malloc(n * n * sizeof(int));
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < n; j++) {
                matrix[i*n + j] = rand() % 10; // valores entre 0 y 9
            }
        }
        for (int j = 0; j < n; j++) {
            vector[j] = rand() % 10;
        }

        // muestra matriz y vector
        printf("Matriz:\n");
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < n; j++) {
                printf("%d ", matrix[i*n + j]);
            }
            printf("\n");
        }
        printf("Vector:\n");
        for (int j = 0; j < n; j++) {
            printf("%d ", vector[j]);
        }
        printf("\n");

        // distribuimos filas de la matriz a cada proceso
        for (int i = 0; i < n; i++) {
            MPI_Send(&matrix[i*n], n, MPI_INT, i, 0, MPI_COMM_WORLD);
        }

        free(matrix);
    }

    // recibe la fila correspondiente
    MPI_Recv(row, n, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

    // replica el vector en todos los procesos
    MPI_Bcast(vector, n, MPI_INT, 0, MPI_COMM_WORLD);

    // calcula producto fila * vector
    result = 0;
    for (int j = 0; j < n; j++) {
        result += row[j] * vector[j];
    }

    // reune resultados en el proceso 0
    int *finalResult = NULL;
    if (rank == 0) {
        finalResult = (int*)malloc(n * sizeof(int));
    }

    MPI_Gather(&result, 1, MPI_INT, finalResult, 1, MPI_INT, 0, MPI_COMM_WORLD);

    // mostramos el resultado final
    if (rank == 0) {
        printf("Resultado (Matriz * Vector):\n");
        for (int i = 0; i < n; i++) {
            printf("%d ", finalResult[i]);
        }
        printf("\n");
        free(finalResult);
    }

    free(row);
    free(vector);

    MPI_Finalize();
    return 0;
}
