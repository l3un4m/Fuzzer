#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include "help.c"

void generate_tar_file(const char *filename) {
    struct tar_t header;
    memset(&header, 0, sizeof(header)); // Zero out struct

    // Fill with some valid values
    strcpy(header.name, "fuzzed.txt");
    strcpy(header.mode, "0000777");
    strcpy(header.uid, "0000000");
    strcpy(header.gid, "0000000");
    sprintf(header.size, "%011o", 10); // 10 bytes file
    strcpy(header.mtime, "00000000000");
    strcpy(header.magic, "ustar");
    strcpy(header.version, "00");

    // Random mutation: corrupt some bytes
    srand(time(NULL));
    int corrupt_byte = rand() % 512;
    ((char*)&header)[corrupt_byte] = rand() % 256;

    // Compute correct checksum
    calculate_checksum(&header);

    // Write to archive.tar
    FILE *fp = fopen(filename, "wb");
    if (!fp) {
        perror("Failed to open file");
        return;
    }
    fwrite(&header, 1, 512, fp);

    char data[10] = "123456789\n";
    fwrite(data, 1, 10, fp);  // Write file data
    fclose(fp);
}
