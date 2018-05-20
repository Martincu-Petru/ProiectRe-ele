#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdio.h>
#include <unistd.h>
#include <ctype.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#define RESET "\x1B[0m"
#define RED "\x1B[31m"
#define GRN "\x1B[32m"
#define BLU "\x1B[34m"
#define CYN "\x1B[36m"

extern void processScn();
extern void errorConfig();
extern void screenWithLogin();

void loginScreen()
{
    int userLogged = 0;
    pid_t PID;
    char user[20];

    system("clear");
    printf(RED "LOGIN SCREEN\n" RESET);
    while (!userLogged)
    {
        printf(CYN "\nEnter username: " RESET);
        scanf("%s", user);
        if ((PID = fork()) == -1)
        {
            perror("Error at fork(): ");
            exit(-1);
        }
        if (PID == 0)
        {
            char readUser[20], litera;
            int user_configFile = open("user_configFile", O_RDONLY), nrChar = -1;
            if(-1 == read(user_configFile, &litera, 1))
            {
                errorConfig();
                exit(-1);
            }
            if (isprint(litera) && !isspace(litera))
                readUser[++nrChar] = litera;
            while (read(user_configFile, &litera, 1) > 0)
            {
                if (isprint(litera) && !isspace(litera))
                    readUser[++nrChar] = litera;
                else
                {
                    readUser[++nrChar] = '\0';
                    if (strcmp(user, readUser) == 0)
                    {
                        userLogged = 1;
                        break;
                    }
                    nrChar = -1;
                }
            }
            if (strcmp(user, readUser) == 0)
                userLogged = 1;
            processScn();
            if (!userLogged)
            {
                printf(RED "\n\nSorry, username not found. Please try again.\n\n" RESET);
                usleep(600000);
                system("clear");
                printf(RED "LOGIN SCREEN\n" RESET);
                exit(0);
            }
            else
            {
                printf(GRN " User found!\n" RESET);
                usleep(600000);
                system("clear");
                exit(1);
            }
        }
        if (PID != 0)
        {
            int status;
            wait(&status);
            userLogged = WEXITSTATUS(status);
            if(userLogged == -1)
            {
                userLogged = 0;
                usleep(600000);
                screenWithLogin();
            }
        }
    }
}