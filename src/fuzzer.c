#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

#include "help.h"


static struct tar_t header;
static struct param_s p_success;
static struct test_s t_success;
char* extract_var;

void fuzz_param(char* parameter, size_t param_size){

    //Empty Values
    reset_tar_header(&header);
    snprintf(parameter, sizeof(param_size), "%s", "");
    create_tar(&header);
    if(extractor(extract_var) == 1){
        t_success.empty_test++;
    }

    //Non-Ascii Value
    unsigned char non_ascii = 0xE2; //First byte of Vietnamese dong
    snprintf(parameter, sizeof(param_size), "%s", &non_ascii);
    create_tar(&header);
    if(extractor(extract_var) == 1){
        t_success.non_ascii_test++;
    }

    //Non-Numeric Value
    char non_numeric[] = "computer-sec";
    reset_tar_header(&header);
    snprintf(parameter, sizeof(param_size), "%s", &non_numeric);
    create_tar(&header);
    if(extractor(extract_var) == 1){
        t_success.non_numeric_test++;
    }

    //Non-Octal Value
    reset_tar_header(&header);
    memset(parameter, '8', param_size - 1);
    parameter[param_size - 1] = 0;
    create_tar(&header);
    if(extractor(extract_var) == 1){
        t_success.non_octal_test++;
    }

    //Null Byte Value
    reset_tar_header(&header);
    memset(parameter, 0, param_size);
    create_tar(&header);
    if(extractor(extract_var) == 1){
        t_success.null_byte_test++;
    }

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

int main(int argc, char* argv[]) {

    extract_var = argv[1];

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

    results(&t_success);
    return 0;
}

