#ifndef FILE_H
#define FILE_H
#include <time.h>

int input_usp_files(char *usp_f[]);
void create_files(int pipfd, char *usp_f[], int num_f);
int cal_age(const char *birth);
void out_results(int pipfd);

#endif
