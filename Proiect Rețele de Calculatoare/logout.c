#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>

#define RESET "\x1B[0m"
#define RED "\x1B[31m"

void logout()
{
    pid_t PID;
    int channel[2];

    if (-1 == pipe(channel))
    {
        perror("Error while creating pipe: ");
        exit(-1);
    }
    if ((PID = fork()) == -1)
    {
        perror("Error while fork(): ");
        exit(-1);
    }
    if (PID == 0)
    {
        close(channel[0]);
        char message[] = "\nLogging out...\n\n";
        message[strlen(message)] = '\0';
        if(-1 == write(channel[1], message, sizeof(message)))
            perror("Error at writing to channel...");
        close(channel[1]);
        exit(0);
    }
    if (PID != 0)
    {
        wait(NULL);
        char character;
        close(channel[1]);
        while (read(channel[0], &character, sizeof(character)) > 0)
            printf(RED "%c" RESET, character);
        if(-1 == read(channel[0], &character, sizeof(character)))
            perror("Error at reading from channel...");
        close(channel[0]);
        usleep(600000);
        system("clear");
    }
}