#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

void processScn()
{
    printf("\nProcessing");
    fflush(stdout);
    usleep(300000);
    printf(".");
    fflush(stdout);
    usleep(300000);
    printf(".");
    fflush(stdout);
    usleep(300000);
    printf(".");
    fflush(stdout);
    usleep(300000);
}