#include <stdlib.h>
#include <dirent.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <time.h>
#include <sys/wait.h>
#include "file.h"

#define MAX_FILENAME 256
#define MAX_FILES 100

int main() {
    int pipfd[2];
    int num_f = 0;
    pid_t pid;
    char *usp_f[MAX_FILES];
    

    num_f = input_usp_files(usp_f);  /*take the list of usp files*/
    if (num_f == 0) {
        printf("can't find USP files in the current directory.\n");
        return 1;
    }

    printf("Find %d .usp files.\n", num_f);

    if (pipe(pipfd) == -1) {     /*create pipe*/
        perror("pipe");
        return 1;
    }

    pid = fork();
    if (pid == -1) {
        perror("fork");
        return 1;
    }

    if (pid == 0) {        /*child process*/
        close(pipfd[0]);
        create_files(pipfd[1], usp_f, num_f);
        close(pipfd[1]);
        exit(0);
    } else {
        close(pipfd[1]);  /*parent process*/
        printf("Parent process waiting...\n");
        wait(NULL);
        printf("Child completed.\n");
        out_results(pipfd[0]);
        close(pipfd[0]);      /*close the pipe*/    
    }

    return 0;
}
