#ifndef NULLFILE_H_
#define NULLFILE_H_

extern "C" {
    #include<stdio.h>
}

FILE * dc_fopen(const char *filename, const char *type);
int dc_fclose(FILE * fl);

#endif