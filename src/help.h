#ifndef _HELP_H
#define _HELP_H

//Struct given by the teacher
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
    //Lines marked as X mean that don't need to be fuzzed
    char devmajor[8];               /* 329 */ //X
    char devminor[8];               /* 337 */ //X
    char prefix[155];               /* 345 */ //X
    char padding[12];               /* 500 */ //X
};

//Struct of the successful tests
struct test_s{
    int empty_test;
    int non_ascii_test;
    int number_test;
    int int_min_test;
    int negative_test;
    int string_test;
    int non_octal_test;
    int null_byte_test;
    int no_null_byte_test;
    int non_expected_value_test;
};

//Functions defined in help.c
int extractor(char* extractor);
unsigned int calculate_checksum(struct tar_t* entry);
void create_tar(struct tar_t *header);
void reset_tar_header(struct tar_t *header);
void results(struct test_s *t_suc);

#endif
