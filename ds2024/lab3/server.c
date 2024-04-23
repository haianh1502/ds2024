#include <stdio.h>
#include <string.h>
#include <mpi.h>

#define MAX_FILENAME_LENGTH 256
#define MAX_BUFFER_SIZE 1024

int main(int argc, char *argv[]) {
    int rank, size;
    char filename[MAX_FILENAME_LENGTH];
    char buffer[MAX_BUFFER_SIZE];

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    if (rank == 0) {
        // Open the file to send
        printf("Enter the filename: ");
        scanf("%s", filename);
        FILE *file = fopen(filename, "r");
        if (file == NULL) {
            printf("Error opening file!\n");
            MPI_Abort(MPI_COMM_WORLD, 1);
        }

        // Get the file size
        fseek(file, 0L, SEEK_END);
        long file_size = ftell(file);
        rewind(file);

        // Broadcast the file size to all processes
        MPI_Bcast(&file_size, 1, MPI_LONG, 0, MPI_COMM_WORLD);

        // Read the file and send it to other processes
        while (!feof(file)) {
            size_t bytes_read = fread(buffer, 1, sizeof(buffer), file);
            MPI_Bcast(buffer, bytes_read, MPI_CHAR, 0, MPI_COMM_WORLD);
        }
        fclose(file);
    } else {
        // Receive the file size from the server
        long file_size;
        MPI_Bcast(&file_size, 1, MPI_LONG, 0, MPI_COMM_WORLD);

        // Create a file to store the received data
        FILE *file = fopen(filename, "w");
        if (file == NULL) {
            printf("Error creating file!\n");
            MPI_Abort(MPI_COMM_WORLD, 1);
        }

        // Receive and write the file data
        while (file_size > 0) {
            int data_size = (file_size > MAX_BUFFER_SIZE) ? MAX_BUFFER_SIZE : file_size;
            MPI_Bcast(buffer, data_size, MPI_CHAR, 0, MPI_COMM_WORLD);
            fwrite(buffer, 1, data_size, file);
            file_size -= data_size;
        }

        fclose(file);
    }

    MPI_Finalize();
    return 0;
}