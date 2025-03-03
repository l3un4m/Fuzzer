#ifndef _HELP_H
#define _HELP_H

 struct tar_t{                      /* byte offset */
    char name[100];                 /*   0 */
    char mode[8];                   /* 100 */
    char uid[8];                    /* 108 */
    char gid[8];                    /* 116 */
    char size[12];                  /* 124 */
    char mtime[12];                 /* 136 */
    char chksum[8];                 /* 148 */
    char typeflag;                  /* 156 */
    char linkname[100];             /* 157 */
    char magic[6];                  /* 257 */
    char version[2];                /* 263 */
    char uname[32];                 /* 265 */
    char gname[32];                 /* 297 */
    char devmajor[8];               /* 329 */ //X
    char devminor[8];               /* 337 */ //X
    char prefix[155];               /* 345 */ //X
    char padding[12];               /* 500 */ //X
    //Lines marked as X mean that don't need to be fuzzed
};

struct successes{
    int name_success;
    int mode_success;
    int uid_success;
    int gid_success;
    int size_success;
    int mtime_success;
    int checksum_success;
    int typeflag_success;
    int linkname_success;
    int magic_success;
    int version_success;
    int uname_success;
    int gname_success;
    int end_of_file_success;
};

int extractor(int argc, char* argv[]);
unsigned int calculate_checksum(struct tar_t* entry);
void create_tar(struct tar_t *header);
void extract_tar(const char *tar_filename);
void reset_tar_header(struct tar_t *header);

#endif
