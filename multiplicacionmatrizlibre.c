#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

int main(int argc, char* argv[]) {
    int rank, size;
    int n; // tamaño de la matriz (n x n)
    int *matrix = NULL, *vector = NULL, *result = NULL;
    int *local_matrix = NULL, *local_result = NULL;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    if (rank == 0) {
        printf("Ingrese el tamaño de la matriz (n x n): ");
        fflush(stdout);
        scanf("%d", &n);

        // Reservar memoria
        matrix = (int*) malloc(n * n * sizeof(int));
        vector = (int*) malloc(n * sizeof(int));
        result = (int*) malloc(n * sizeof(int));

        // Inicializar matriz y vector con valores simples
        printf("Matriz:\n");
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < n; j++) {
                matrix[i*n + j] = i + j + 1; 
                printf("%d ", matrix[i*n + j]);
            }
            printf("\n");
        }

        printf("Vector:\n");
        for (int i = 0; i < n; i++) {
            vector[i] = 1;
            printf("%d ", vector[i]);
        }
        printf("\n");
    }

    // Broadcast del tamaño n
    MPI_Bcast(&n, 1, MPI_INT, 0, MPI_COMM_WORLD);

    // Broadcast del vector completo
    if (rank != 0) {
        vector = (int*) malloc(n * sizeof(int));
    }
    MPI_Bcast(vector, n, MPI_INT, 0, MPI_COMM_WORLD);

    // Calcular distribución de filas (cuántas a cada proceso)
    int base_rows = n / size;
    int extra = n % size;

    int *sendcounts = (int*) malloc(size * sizeof(int));
    int *displs = (int*) malloc(size * sizeof(int));
    int *recvcounts = (int*) malloc(size * sizeof(int));
    int *recvdispls = (int*) malloc(size * sizeof(int));

    int offset = 0;
    for (int p = 0; p < size; p++) {
        int rows_p = (p < extra) ? base_rows + 1 : base_rows;
        sendcounts[p] = rows_p * n;   // cantidad de elementos que envía al proceso p
        displs[p] = offset * n;       // desplazamiento dentro de la matriz
        recvcounts[p] = rows_p;       // cantidad de resultados que recibe
        recvdispls[p] = offset;       // desplazamiento dentro del vector resultado
        offset += rows_p;
    }

    int local_rows = (rank < extra) ? base_rows + 1 : base_rows;
    local_matrix = (int*) malloc(local_rows * n * sizeof(int));
    local_result = (int*) malloc(local_rows * sizeof(int));

    // Distribuir filas con Scatterv
    MPI_Scatterv(matrix, sendcounts, displs, MPI_INT,
                 local_matrix, local_rows * n, MPI_INT, 0, MPI_COMM_WORLD);

    // Multiplicar filas locales por el vector
    for (int i = 0; i < local_rows; i++) {
        local_result[i] = 0;
        for (int j = 0; j < n; j++) {
            local_result[i] += local_matrix[i*n + j] * vector[j];
        }
    }

    // Recolectar resultados con Gatherv
    if (rank == 0) {
        MPI_Gatherv(local_result, local_rows, MPI_INT,
                    result, recvcounts, recvdispls, MPI_INT,
                    0, MPI_COMM_WORLD);

        printf("Resultado (matriz x vector):\n");
        for (int i = 0; i < n; i++) {
            printf("%d ", result[i]);
        }
        printf("\n");
    } else {
        MPI_Gatherv(local_result, local_rows, MPI_INT,
                    NULL, NULL, NULL, MPI_INT,
                    0, MPI_COMM_WORLD);
    }

    // Liberar memoria
    free(local_matrix);
    free(local_result);
    free(vector);
    free(sendcounts);
    free(displs);
    free(recvcounts);
    free(recvdispls);

    if (rank == 0) {
        free(matrix);
        free(result);
    }

    MPI_Finalize();
    return 0;
}
