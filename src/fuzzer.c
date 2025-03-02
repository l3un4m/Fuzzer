#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include "help.c"

#define BLOCK_SIZE 512

static struct tar_t header;

// Function to create a tar file using a given header
void create_tar(struct tar_t *header) {
    FILE *file = fopen("test.tar", "wb");
    if (!file) {
        perror("Failed to create tar file");
        return;
    }

    // Compute and set the checksum for the given header
    calculate_checksum(header);

    // Write header
    fwrite(header, 1, BLOCK_SIZE, file);

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
    printf("Tar file 'test.tar' created successfully with provided header!\n");
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

// Function to reset an existing tar header to default values
void reset_tar_header(struct tar_t *header) {
    memset(header, 0, BLOCK_SIZE);

    snprintf(header->name, sizeof(header->name), "default.txt");
    snprintf(header->mode, sizeof(header->mode), "%07o", 0644);
    snprintf(header->uid, sizeof(header->uid), "%07o", 0);
    snprintf(header->gid, sizeof(header->gid), "%07o", 0);
    snprintf(header->size, sizeof(header->size), "%011o", 1024);
    snprintf(header->mtime, sizeof(header->mtime), "%011o", 0);
    header->typeflag = '0'; // Regular file
    strcpy(header->magic, "ustar");
    strcpy(header->version, "00");

    // Compute and set the checksum
    calculate_checksum(header);
}

void fuzz_field(char* field_name, size_t field_size){

    //Empty Values
    reset_tar_header(&header);
    snprintf(header.name, sizeof(header.name), "%s", "");
    create_tar(&header);
    extract_tar("test.tar");
}

void fuzz_name() { fuzz_field(header.name, sizeof(header.name)); }
void fuzz_mode() { fuzz_field(header.mode, sizeof(header.mode)); }
void fuzz_uid() { fuzz_field(header.uid, sizeof(header.uid)); }
void fuzz_gid() { fuzz_field(header.gid, sizeof(header.gid)); }
void fuzz_size() { fuzz_field(header.size, sizeof(header.size)); }
void fuzz_mtime() { fuzz_field(header.mtime, sizeof(header.mtime)); }
void fuzz_chksum() { fuzz_field(header.chksum, sizeof(header.chksum)); }
void fuzz_typeflag() { fuzz_field(&header.typeflag, sizeof(header.typeflag)); }
void fuzz_linkname() { fuzz_field(header.linkname, sizeof(header.linkname)); }
void fuzz_magic() { fuzz_field(header.magic, sizeof(header.magic)); }
void fuzz_version() { fuzz_field(header.version, sizeof(header.version)); }
void fuzz_uname() { fuzz_field(header.uname, sizeof(header.uname)); }
void fuzz_gname() { fuzz_field(header.gname, sizeof(header.gname)); }

int main() {
    fuzz_name();
    fuzz_mode();
    fuzz_uid();
    fuzz_gid();
    fuzz_size();
    fuzz_mtime();
    fuzz_chksum();
    fuzz_typeflag();
    fuzz_linkname();
    fuzz_magic();
    fuzz_version();
    fuzz_uname();
    fuzz_gname();
    return 0;
}

