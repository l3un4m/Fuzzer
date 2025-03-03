#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

#include "help.h"


static struct tar_t header;

void fuzz_param(char* parameter, size_t param_size){

    //Empty Values
    reset_tar_header(&header);
    snprintf(parameter, sizeof(param_size), "%s", "");
    create_tar(&header);
    extract_tar("test.tar");

    //Non-Ascii Value
    unsigned char non_ascii = 0xE2; //First byte of Vietnamese dong
    snprintf(parameter, sizeof(param_size), "%s", &non_ascii);
    create_tar(&header);
    extract_tar("test.tar");

    //Non-Numeric Value
    char non_numeric[] = "computer-sec";
    reset_tar_header(&header);
    snprintf(parameter, sizeof(param_size), "%s", &non_numeric);
    create_tar(&header);
    extract_tar("test.tar");

    //Non-Octal Value
    reset_tar_header(&header);
    memset(parameter, '8', param_size - 1);
    parameter[param_size - 1] = 0;
    create_tar(&header);
    extract_tar("test.tar");

    //Null Byte Value
    reset_tar_header(&header);
    memset(parameter, 0, param_size);
    create_tar(&header);
    extract_tar("test.tar");

}


void fuzz_name()    { fuzz_param(header.name, sizeof(header.name)); }
void fuzz_mode()    { fuzz_param(header.mode, sizeof(header.mode)); }
void fuzz_uid()     { fuzz_param(header.uid, sizeof(header.uid)); }
void fuzz_gid()     { fuzz_param(header.gid, sizeof(header.gid)); }
void fuzz_size()    { fuzz_param(header.size, sizeof(header.size)); }
void fuzz_mtime()   { fuzz_param(header.mtime, sizeof(header.mtime)); }
void fuzz_chksum()  { fuzz_param(header.chksum, sizeof(header.chksum)); }
void fuzz_typeflag(){ fuzz_param(&header.typeflag, sizeof(header.typeflag)); }
void fuzz_linkname(){ fuzz_param(header.linkname, sizeof(header.linkname)); }
void fuzz_magic()   { fuzz_param(header.magic, sizeof(header.magic)); }
void fuzz_version() { fuzz_param(header.version, sizeof(header.version)); }
void fuzz_uname()   { fuzz_param(header.uname, sizeof(header.uname)); }
void fuzz_gname()   { fuzz_param(header.gname, sizeof(header.gname)); }

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

