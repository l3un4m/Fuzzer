#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <unistd.h>
#include <limits.h>

#include "help.h"

//Global Variables
static struct tar_t header;     //Tar Header
static struct test_s t_success; //Successful Tests
char* extract_var;              //Path to the extractor
int success_count = 0;          //Number of Successful Tests

//  Function that fuzz and crash an extractor, inside are dedicated mini-fuzzers
//  Takes a tar header parameter and it's corresponding size to fuzz
//  Structure of each test is simillar, reset the global header, fuzz it, create and attempt to extract tar resulting in a crash and if so update the successful tests struct
void fuzz_param(char* parameter, size_t param_size){

    char new_filename[50]; //To rename the successful tars


    //Changes parameter to be empty
    reset_tar_header(&header);
    snprintf(parameter, param_size, "%s", "");
    create_tar(&header);
    if(extractor(extract_var) == 1){
        t_success.empty_test++;
        success_count++;
        snprintf(new_filename, sizeof(new_filename), "success_%d.tar", success_count++);
        rename("archive.tar", new_filename);
    }
    else{remove("archive.tar");}

    //Changes parameter to only contain a non-ascii character
    reset_tar_header(&header);
    unsigned char non_ascii = 0xE2; //First byte of Vietnamese dong
    snprintf(parameter, param_size, "%s", &non_ascii);
    create_tar(&header);
    if(extractor(extract_var) == 1){
        t_success.non_ascii_test++;
        success_count++;
        snprintf(new_filename, sizeof(new_filename), "success_%d.tar", success_count++);
        rename("archive.tar", new_filename);
    }
    //Changes parameter to contain integer value
    reset_tar_header(&header);
    snprintf(parameter, param_size, "%d", 1);
    create_tar(&header);
    if(extractor(extract_var) == 1){
        t_success.number_test++;
        success_count++;
        snprintf(new_filename, sizeof(new_filename), "success_%d.tar", success_count++);
        rename("archive.tar", new_filename);
    }

    //Changes parameter to contain the smallest integer value
    reset_tar_header(&header);
    snprintf(parameter, param_size, "%d", INT_MIN);
    create_tar(&header);
    if(extractor(extract_var) == 1){
        t_success.int_min_test++;
        success_count++;
        snprintf(new_filename, sizeof(new_filename), "success_%d.tar", success_count++);
        rename("archive.tar", new_filename);
    }

    //Changes parameter to contain negative value
    reset_tar_header(&header);
    snprintf(parameter, param_size, "%d", -1);
    create_tar(&header);
    if(extractor(extract_var) == 1){
        t_success.negative_test++;
        success_count++;
        snprintf(new_filename, sizeof(new_filename), "success_%d.tar", success_count++);
        rename("archive.tar", new_filename);
    }

    //Changes parameter to contain a string(focused on the header parameters that aren't strings)
    reset_tar_header(&header);
    char string[] = "computer-sec";
    snprintf(parameter, param_size, "%s", &string);
    create_tar(&header);
    if(extractor(extract_var) == 1){
        t_success.string_test++;
        success_count++;
        snprintf(new_filename, sizeof(new_filename), "success_%d.tar", success_count++);
        rename("archive.tar", new_filename);
    }
    else{remove("archive.tar");}

    //Changes parameter to contain a non-octal value (focused on fields that need to have octal values: mode, uid, gid, size and mtime)
    reset_tar_header(&header);
    memset(parameter, '8', param_size - 1);
    parameter[param_size - 1] = 0;
    create_tar(&header);
    if(extractor(extract_var) == 1){
        t_success.non_octal_test++;
        success_count++;
        snprintf(new_filename, sizeof(new_filename), "success_%d.tar", success_count++);
        rename("archive.tar", new_filename);
    }
    else{remove("archive.tar");}

    //Changes parameter to contain NULL values
    reset_tar_header(&header);
    memset(parameter, 0, param_size);
    create_tar(&header);
    if(extractor(extract_var) == 1){
        t_success.null_byte_test++;
        success_count++;
        snprintf(new_filename, sizeof(new_filename), "success_%d.tar", success_count++);
        rename("archive.tar", new_filename);
    }
    else{remove("archive.tar");}

    //Changes parameter to end in a non Null byte
    reset_tar_header(&header);
    memset(parameter, '0', param_size);
    create_tar(&header);
    if(extractor(extract_var) == 1){
        t_success.no_null_byte_test++;
        success_count++;
        snprintf(new_filename, sizeof(new_filename), "success_%d.tar", success_count++);
        rename("archive.tar", new_filename);
    }
    else{remove("archive.tar");}

    //Changes parameter to reserved strings
    char non_expected[] = {'\b', '\f', '\n', '\r', '\t', '\v','\"', '\'', ' '};

    for(int i = 0; i < sizeof(non_expected); i++){

        reset_tar_header(&header);
        memset(parameter, non_expected[i], param_size - 1);
        create_tar(&header);
        if(extractor(extract_var) == 1){
            t_success.non_expected_value_test++;
            success_count++;
            snprintf(new_filename, sizeof(new_filename), "success_%d.tar", success_count++);
            rename("archive.tar", new_filename);
        }
        else{remove("archive.tar");}



    }


}

//  All the next functions have the same purpose: Use the previous fuzzer on each specific parameter
//  Take as argument the parameter ID and it's size
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

//  Main function, fuzzes globally and shows the results
//  Takes the patch of the extractor as argument
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
    remove("delete.tar"); //For some reason one delete.tar persists when it should be deleted


    return 0;
}

