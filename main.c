#include <stdio.h>
#include <sys/fcntl.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdlib.h>
#include "string.h"

//help function that reads a line from a file and returns it and its size
char *read_line(int fd, char *buff , int *size) {
    int i = 0;
    char c;
    while (read(fd, &c, 1) > 0) {
        if (c == ' ')
            break;
        buff[i] = c;
        i++;
    }
    *size = i;
    buff[i] = '\0';
    return buff;
}

int main(int argc, char *argv[]) {
    open("grades.cvs", O_WRONLY | O_RDONLY | O_CREAT, 0777);

    int config = open("/Users/delouya.thefist/CLionProjects/Exam_1/CMakeLists copy.txt", O_RDONLY);

    char *dir = (char *) malloc(100);
    char *in = (char *) malloc(100);
    char *out = (char *) malloc(100);

    int i = 1;
    int size = 0;

    //read config file char by char into dir, in, out
    while (i < 4) {
        if (i == 1) {
            read_line(config, dir, &size);
        }
        if (i == 2) {
            read_line(config, in, &size);
        }
        if (i == 3) {
            read_line(config, out, &size);
        }
        i++;
    }


    printf("%s\n%s\n%s", dir, in, out);
    close(config);


    pid_t pid;
    if ((pid = fork()) == 0) {
        //child
        close(1);
        int fd = open("studentList.txt", O_WRONLY | O_CREAT | O_RDONLY, 0777);
        dup(fd);
        char **args = (char **) malloc(3 * sizeof(char *));
        args[0] = "ls";
        args[1] = (char *) malloc(50);
        strcpy(args[1], dir);
        args[2] = NULL;
        execvp("ls", args);
        close(fd);
    } else {
        //parent
        wait(NULL);
        int fd = open("studentList.txt", O_WRONLY | O_RDONLY  | O_CREAT , 0777);
        char *buf = (char *) malloc(50);
        int size = 0;
        //read all students names into array
        char **students = (char **) malloc(100 * sizeof(char *));
        int i = 0;
        while (read_line(fd, buf, &size) != NULL) {
            students[i] = (char *) malloc(50);
            strcpy(students[i], buf);
            i++;
        }
        close(fd);

        //for each student create a new process and run the student .exe with the given input
        for (int j = 0; j < i; j++) {
            if ((pid = fork()) == 0) {
                //child
                int fd = open("out.txt", O_WRONLY | O_CREAT | O_RDONLY, 0777);
                close(1);
                dup(fd);
                close(fd);
                char **args = (char **) malloc(5 * sizeof(char *));
                args[0] = (char *) malloc(50);
                strcpy(args[0], "./allStudents/");
                strcat(args[0], students[j]);
                strcat(args[0], "/main.exe");
                args[1] = (char *) malloc(50);
                strcpy(args[1], in);
                args[2] = NULL;
                execvp(args[0], args);
            }
            else{
                //parent
                //if the child process finished successfully, compare the output to the expected output
                wait(NULL);
                int fd = open("out.txt", O_WRONLY | O_RDONLY  | O_CREAT , 0777);
                char *buf = (char *) malloc(50);
                int size = 0;
                char *out = (char *) malloc(50);
                read_line(fd, out, &size);
                close(fd);

                //compare the output to the expected output


            }

        }
    }
}






    //run in a loop the size of config, and read char by char into dir, in, out







