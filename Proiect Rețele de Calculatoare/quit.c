#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

#define RED "\x1B[31m"
#define RESET "\x1B[0m"

void quit()
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
        char message[] = "\nQuitting\n\n";
        message[strlen(message)] = '\0';
        if(-1 == write(channel[1], message, sizeof(message)))
            perror("Error at writing in pipe...");
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
            perror("Error at reading from pipe...");
        close(channel[0]);
        usleep(600000);
        system("clear");
        exit(0);
    }
}