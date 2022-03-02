#ifndef NULLFILE_H_
#define NULLFILE_H_

FILE * NULL_FILE = 0;

FILE * dc_fopen(const char *filename, const char *type);
int dc_fclose(FILE * fl);

#endif