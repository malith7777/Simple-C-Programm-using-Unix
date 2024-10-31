#include <stdlib.h>
#include <dirent.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <time.h>
#include "file.h"

#define MAX_NAME 100
#define MAX_LINE 100
#define MAX_FILENAME 256


int input_usp_files(char *usp_f[]) {
    DIR *dir;
    int cou = 0;
    struct dirent *entry;

    dir = opendir(".");  /*open the directory*/
    if (dir == NULL) {
        perror("opendir");
        return 0;
    }

    while ((entry = readdir(dir)) != NULL) {   /*identify usp files+*/
        if (strstr(entry->d_name, ".usp") != NULL) {
            usp_f[cou] = malloc(strlen(entry->d_name) + 1);
            strcpy(usp_f[cou], entry->d_name);
            cou++;
        }
    }

    closedir(dir);
    return cou;
}

void create_files(int pipfd, char *usp_f[], int num_f) {
    char birth[MAX_LINE];
    char N[MAX_NAME];
    char r[MAX_NAME + 20];
    int fd, Age;
    char buff[2];
    int x, y;
    ssize_t bytes_r;
    
    for (x = 0; x < num_f; x++) {
        printf("Process file: %s\n", usp_f[x]);

        fd = open(usp_f[x], O_RDONLY);  /*open the file*/
        if (fd == -1) {
            perror("open");
            continue;
        }

        
        y = 0;    /*read the name line*/
        while ((bytes_r = read(fd, buff, 1)) > 0 && buff[0] != '\n' && y < MAX_NAME - 1) {
            N[y++] = buff[0];
        }
        N[y] = '\0';  

        
        y = 0;
        while ((bytes_r = read(fd, buff, 1)) > 0 && buff[0] != '\n' && y < MAX_LINE - 1) {
            birth[y++] = buff[0];
        }
        birth[y] = '\0';  

        printf("Birth: %s\n", birth);

        Age = cal_age(birth);     
        printf("Name: %s, Age: %d\n", N, Age);

        sprintf(r, "%s:%d\n", N, Age);
        write(pipfd, r, strlen(r));  /*write result to the pipe*/

        close(fd);
    }
}

int cal_age(const char *birth) { /*calculate the age*/
    struct tm tm_birth = {0};
    struct tm *tm_now;
    char recent_date[11];
    time_t now;

    sscanf(birth, "%d-%d-%d", &tm_birth.tm_mday, &tm_birth.tm_mon, &tm_birth.tm_year);
    tm_birth.tm_mon--;
    tm_birth.tm_year -= 1900;
    
    time(&now);
    tm_now = localtime(&now);

    strftime(recent_date, sizeof(recent_date), "%d-%m-%Y", tm_now);
    printf("recent Date: %s\n", recent_date);
    
    int Age = tm_now->tm_year - tm_birth.tm_year;

    if (tm_now->tm_mon < tm_birth.tm_mon ||
        (tm_now->tm_mon == tm_birth.tm_mon && tm_now->tm_mday < tm_birth.tm_mday)) {
        Age--;
    }
    
    printf("Cal Age: %d\n", Age);
    
    return Age;
}

void out_results(int pipfd) {    /*open the result.txt*/
    char r[MAX_NAME + 20];
    int fd = open("result.txt", O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (fd == -1) {
        perror("open");
        return;
    }

    while (read(pipfd, r, sizeof(r)) > 0) {
        write(fd, r, strlen(r));
    }

    close(fd);
    
}
