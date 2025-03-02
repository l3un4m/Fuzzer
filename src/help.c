#include <stdio.h>
#include <string.h>

 #define BLOCK_SIZE 512

struct tar_t
{                              /* byte offset */
    char name[100];               /*   0 */
    char mode[8];                 /* 100 */
    char uid[8];                  /* 108 */
    char gid[8];                  /* 116 */
    char size[12];                /* 124 */
    char mtime[12];               /* 136 */
    char chksum[8];               /* 148 */
    char typeflag;                /* 156 */
    char linkname[100];           /* 157 */
    char magic[6];                /* 257 */
    char version[2];              /* 263 */
    char uname[32];               /* 265 */
    char gname[32];               /* 297 */
    char devmajor[8];             /* 329 */ //X
    char devminor[8];             /* 337 */ //X
    char prefix[155];             /* 345 */ //X
    char padding[12];             /* 500 */ //X
    //Lines marked as X mean that don't need to be fuzzed
};

/**
 * Launches another executable given as argument,
 * parses its output and check whether or not it matches "*** The program has crashed ***".
 * @param the path to the executable
 * @return -1 if the executable cannot be launched,
 *          0 if it is launched but does not print "*** The program has crashed ***",
 *          1 if it is launched and prints "*** The program has crashed ***".
 *
 * BONUS (for fun, no additional marks) without modifying this code,
 * compile it and use the executable to restart our computer.
 */
int mainn(int argc, char* argv[])
{
    if (argc < 2)
        return -1;
    int rv = 0;
    char cmd[51];
    strncpy(cmd, argv[1], 25);
    cmd[26] = '\0';
    strncat(cmd, " archive.tar", 25);
    char buf[33];
    FILE *fp;

    if ((fp = popen(cmd, "r")) == NULL) {
        printf("Error opening pipe!\n");
        return -1;
    }

    if(fgets(buf, 33, fp) == NULL) {
        printf("No output\n");
        goto finally;
    }
    if(strncmp(buf, "*** The program has crashed ***\n", 33)) {
        printf("Not the crash message\n");
        goto finally;
    } else {
        printf("Crash message\n");
        rv = 1;
        goto finally;
    }
    finally:
    if(pclose(fp) == -1) {
        printf("Command not found\n");
        rv = -1;
    }
    return rv;
}

/**
 * Computes the checksum for a tar header and encode it on the header
 * @param entry: The tar header
 * @return the value of the checksum
 */
unsigned int calculate_checksum(struct tar_t* entry){
    // use spaces for the checksum bytes while calculating the checksum
    memset(entry->chksum, ' ', 8);

    // sum of entire metadata
    unsigned int check = 0;
    unsigned char* raw = (unsigned char*) entry;
    for(int i = 0; i < 512; i++){
        check += raw[i];
    }

    snprintf(entry->chksum, sizeof(entry->chksum), "%06o0", check);

    entry->chksum[6] = '\0';
    entry->chksum[7] = ' ';
    return check;
}

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
