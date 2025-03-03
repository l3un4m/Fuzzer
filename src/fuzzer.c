#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include "help.c"


static struct tar_t header;

void fuzz_field(char* parameter, size_t param_size){
/*
    //Empty Values
    reset_tar_header(&header);
    snprintf(parameter, sizeof(param_size), "%s", "");
    create_tar(&header);
    extract_tar("test.tar");

    //Non-Ascii Value
    unsigned char non_ascii = 0xE2; //First byte of dong
    reset_tar_header(&header);
    snprintf(parameter, sizeof(param_size), "%s", &non_ascii);
    create_tar(&header);
    extract_tar("test.tar");

    //Non-Numeric Value
    char non_numeric[] = "computer-sec";
    reset_tar_header(&header);
    snprintf(parameter, sizeof(param_size), "%s", &non_numeric);
    create_tar(&header);
    extract_tar("test.tar");
*/
    //Non-Octal Value
    reset_tar_header(&header);
    memset(parameter, '8', param_size - 1);
    parameter[param_size - 1] = 0;
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

