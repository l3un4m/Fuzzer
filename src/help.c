#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include "help.h"

 #define BLOCK_SIZE 512


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
int extractor(char* extractor)
{
    int rv = 0;
    char cmd[51];
    strncpy(cmd, extractor, 25);
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

//  Function to create a tar file using a given header
//  Takes a pointer to a header struct as argument
void create_tar(struct tar_t *header) {
    FILE *file = fopen("archive.tar", "wb");
    if (!file) {
        perror("Failed to create tar file");
        return;
    }

    // Compute and set the checksum for the given header
    calculate_checksum(header);

    // Write header
    fwrite(header, 1, BLOCK_SIZE, file);

    // Write two empty blocks at the end of archive
    char empty_block[BLOCK_SIZE] = {0};
    fwrite(empty_block, 1, BLOCK_SIZE, file);
    fwrite(empty_block, 1, BLOCK_SIZE, file);

    fclose(file);

}

//  Function to reset an existing tar header to default values
//  Takes a pointer to a header struct as argument
void reset_tar_header(struct tar_t *header) {
    char tar_filename[100];
    snprintf(tar_filename, sizeof(tar_filename), "%s", "delete.tar"); //Sometimes some residue tars appear
    char symbolic_link[100];
    memset(symbolic_link, 'k', sizeof(symbolic_link) - 1);
    symbolic_link[99] = '\0';
    char padding[8] = "1111111";

    memset(header, 0, sizeof(struct tar_t));

    snprintf(header->name, sizeof(header->name), "%s", tar_filename);
    snprintf(header->mode, sizeof(header->mode), "07777"); // Read & execute perms
    snprintf(header->uid, sizeof(header->uid), "%s", padding);
    snprintf(header->gid, sizeof(header->gid), "%s", padding);
    snprintf(header->size, sizeof(header->size), "%011o", 512); // Default block size in octal
    snprintf(header->mtime, sizeof(header->mtime), "%011lo", time(NULL)); // Current time in octal

    header->typeflag = '1'; // Hard link type
    snprintf(header->linkname, sizeof(header->linkname), "%s", symbolic_link);
    snprintf(header->magic, sizeof(header->magic), "ustar");
    snprintf(header->version, sizeof(header->version) + 1, "00");
    snprintf(header->uname, sizeof(header->uname), "RANDOM");
    snprintf(header->gname, sizeof(header->gname), "RANDOM_GROUP");
    snprintf(header->devmajor, sizeof(header->devmajor), "%s", padding);
    snprintf(header->devminor, sizeof(header->devminor), "%s", padding);

    calculate_checksum(header);
}

//  Function that will print the results of our Fuzzer
//  Takes a pointer to a successful tests struct as argument
void results(struct test_s *t_suc){
    printf("\nResults for each Test\n");
    printf("Empty:%d\n",t_suc->empty_test);
    printf("Non-Ascii:%d\n", t_suc->non_ascii_test);
    printf("String:%d\n", t_suc->string_test);
    printf("Non-Octal:%d\n", t_suc->non_octal_test);
    printf("Null-Byte:%d\n", t_suc->null_byte_test);
    printf("No-Null-Byte:%d\n", t_suc->no_null_byte_test);
    printf("Non-Expected:%d\n", t_suc->non_expected_value_test);
}




