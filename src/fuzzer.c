#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include "help.c"

#define BLOCK_SIZE 512


// Function to create a tar file with "Hello, World!" content
void create_tar(const char *filename) {
    FILE *file = fopen("test.tar", "wb");
    if (!file) {
        perror("Failed to create tar file");
        return;
    }

    struct tar_t header;
    memset(&header, 0, BLOCK_SIZE);

    // Set file metadata
    snprintf(header.name, sizeof(header.name), "%s", filename);
    snprintf(header.mode, sizeof(header.mode), "%07o", 0644);
    snprintf(header.uid, sizeof(header.uid), "%07o", 0);
    snprintf(header.gid, sizeof(header.gid), "%07o", 0);
    snprintf(header.size, sizeof(header.size), "%011o", 13); // "Hello, World!" length
    snprintf(header.mtime, sizeof(header.mtime), "%011o", 0);
    header.typeflag = '0'; // Regular file
    strcpy(header.magic, "ustar");
    strcpy(header.version, "00");

    // Compute and set the checksum
    calculate_checksum(&header);

    // Write header
    fwrite(&header, 1, BLOCK_SIZE, file);

    // Write "Hello, World!" content
    const char *content = "Hello, World!";
    fwrite(content, 1, 13, file);

    // Pad file content to a multiple of BLOCK_SIZE
    size_t padding_size = BLOCK_SIZE - (13 % BLOCK_SIZE);
    if (padding_size != BLOCK_SIZE) {
        char padding[BLOCK_SIZE] = {0};
        fwrite(padding, 1, padding_size, file);
    }

    // Write two empty blocks to signify end of archive
    char empty_block[BLOCK_SIZE] = {0};
    fwrite(empty_block, 1, BLOCK_SIZE, file);
    fwrite(empty_block, 1, BLOCK_SIZE, file);

    fclose(file);
    printf("Tar file 'test.tar' created successfully with 'Hello, World!' inside!\n");
}

// Function to extract a tar file using the extractor binary
void extract_tar(const char *tar_filename) {
    char command[256];
    snprintf(command, sizeof(command), "./extractor_x86_64 %s", tar_filename);

    int result = system(command);
    if (result == -1) {
        perror("Failed to execute extractor");
    } else {
        printf("Extractor executed with exit code: %d\n", result);
    }
}

int main() {
    create_tar("hello.txt");
    extract_tar("test.tar");
    return 0;
}

