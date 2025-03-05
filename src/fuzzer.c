#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <unistd.h>

#include "help.h"


static struct tar_t header;
static struct param_s p_success;
static struct test_s t_success;
char* extract_var;
int success_count = 0;

void fuzz_param(char* parameter, size_t param_size){

    char new_filename[50];


    //Empty Values
    reset_tar_header(&header);
    snprintf(parameter, param_size, "%s", "");
    create_tar(&header);
    if(extractor(extract_var) == 1){
        t_success.empty_test++;
        success_count++;
        snprintf(new_filename, sizeof(new_filename), "success%d.tar", success_count++);
        rename("archive.tar", new_filename);
    }
    else{remove("archive.tar");}

    //Non-Ascii Value
    unsigned char non_ascii = 0xE2; //First byte of Vietnamese dong
    snprintf(parameter, param_size, "%s", &non_ascii);
    create_tar(&header);
    if(extractor(extract_var) == 1){
        t_success.non_ascii_test++;
        success_count++;
        snprintf(new_filename, sizeof(new_filename), "success%d.tar", success_count++);
        rename("archive.tar", new_filename);
    }

    //Non-Numeric Value
    char non_numeric[] = "computer-sec";
    reset_tar_header(&header);
    snprintf(parameter, param_size, "%s", &non_numeric);
    create_tar(&header);
    if(extractor(extract_var) == 1){
        t_success.non_numeric_test++;
        success_count++;
        snprintf(new_filename, sizeof(new_filename), "success%d.tar", success_count++);
        rename("archive.tar", new_filename);
    }
    else{remove("archive.tar");}

    //Non-Octal Value
    reset_tar_header(&header);
    memset(parameter, '8', param_size - 1);
    parameter[param_size - 1] = 0;
    create_tar(&header);
    if(extractor(extract_var) == 1){
        t_success.non_octal_test++;
        success_count++;
        snprintf(new_filename, sizeof(new_filename), "success%d.tar", success_count++);
        rename("archive.tar", new_filename);
    }
    else{remove("archive.tar");}

    //Null Byte Value
    reset_tar_header(&header);
    memset(parameter, 0, param_size);
    create_tar(&header);
    if(extractor(extract_var) == 1){
        t_success.null_byte_test++;
        success_count++;
        snprintf(new_filename, sizeof(new_filename), "success%d.tar", success_count++);
        rename("archive.tar", new_filename);
    }
    else{remove("archive.tar");}

    //Parameter finished with non Null byte
    reset_tar_header(&header);
    memset(parameter, '0', param_size);
    create_tar(&header);
    if(extractor(extract_var) == 1){
        t_success.no_null_byte_test++;
        success_count++;
        snprintf(new_filename, sizeof(new_filename), "success%d.tar", success_count++);
        rename("archive.tar", new_filename);
    }
    else{remove("archive.tar");}

    //Non-Expected Values
    char non_expected[] = {'\t', '\r', '\n', '\v', '\f', '\b'};

    for(int i = 0; i < sizeof(non_expected); i++){

        reset_tar_header(&header);
        memset(parameter, non_expected[i], param_size - 1);
        create_tar(&header);
        if(extractor(extract_var) == 1){
            t_success.non_expected_value_test++;
            success_count++;
            snprintf(new_filename, sizeof(new_filename), "success%d.tar", success_count++);
            rename("archive.tar", new_filename);
        }
        else{remove("archive.tar");}



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
    printf("Successful Tars Created:%d\n", success_count);
    sleep(2); //To make the inginious not go crazy
    return 0;
}

