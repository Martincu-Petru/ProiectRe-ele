#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>

#define FIFO_NAME "MyTest_FIFO"
#define RESET "\x1B[0m"
#define RED "\x1B[31m"

extern void processDirectory(const char *path, char fileName[], int FD, int *findAtLeast);

void myfind(char fileName[])
{
    int FD, findAtLeast;
    pid_t PID;
    mknod(FIFO_NAME, S_IFIFO | 0666, 0);
    if (-1 == (PID = fork()))
    {
        perror("Error while fork(): ");
        exit(-1);
    }
    if (PID == 0)
    {
        if (-1 == (FD = open(FIFO_NAME, O_WRONLY)))
        {
            perror("Error at opening FIFO file: ");
            exit(-1);
        }
        processDirectory(".", fileName, FD, &findAtLeast);
        if(!findAtLeast)
            printf(RED "\nError 404! File %s not found!\n\n" RESET, fileName);
        exit(0);
    }
    if (PID != 0)
    {
        char character;
        if (-1 == (FD = open(FIFO_NAME, O_RDONLY)))
        {
            perror("Error at opening FIFO file: ");
            exit(-1);
        }
        if (-1 == read(FD, &character, 1))
        {
            perror("Error at reading from FIFO file: ");
            exit(-1);
        }
        else
        {
            while (read(FD, &character, 1) > 0)
                printf("%c", character);
        }
        unlink("MyTest_FIFO");
    }
}