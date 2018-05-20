#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>
#include <ctype.h>
#include <string.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <dirent.h>
#include <time.h>

#define RESET "\x1B[0m"
#define RED "\x1B[31m"

extern void processScn();
extern void loginScreen();
extern void quit();
extern void myfind(char fileName[]);
extern int processCommand(char commandAux[], char command[], char fileName[], int *logQuitLogout);
extern void screenWithLogin();
extern void screenWithLogout();
extern void logout();
extern void errorScreen();
extern void commandsScreen();
extern void errorCommand();
extern void errorLogout();
extern void processDirectoryNonRecursive(char fileName[]);
extern void errorLogQuit();

int isUserLogged = 0;

int main()
{
    pid_t PID;

    char command[256], commandAux[256], fileName[256];

    int logQuitLogout;

    screenWithLogin();

    while (fgets(command, sizeof(command), stdin) != NULL)
    {
        if ((-1 == processCommand(commandAux, command, fileName, &logQuitLogout)))
        {
            if(logQuitLogout)
            {
                errorLogQuit();
                usleep(600000);
                system("clear");
                if(isUserLogged)
                {
                    screenWithLogout();
                    continue;
                }
                else if(!isUserLogged)
                { 
                    screenWithLogin();
                    continue;
                }
            }
            if (isUserLogged)
            {
                errorCommand();
                commandsScreen();
                continue;
            }
        }
        if (!strcmp(commandAux, "quit"))
        {
            quit();
        }
        else if (!strcmp(commandAux, "myfind") && isUserLogged)
        {
            myfind(fileName);
            usleep(1000);
            commandsScreen();
        }
        else if (!strcmp(commandAux, "myfind") && !isUserLogged)
        {
            errorScreen();
        }
        else if (!strcmp(commandAux, "mystat") && isUserLogged)
        {
            processDirectoryNonRecursive(fileName);
            usleep(1000);
            commandsScreen();
        }
        else if (!strcmp(commandAux, "mystat") && !isUserLogged)
        {
            errorScreen();
        }
        else if (!strcmp(commandAux, "login"))
        {
            loginScreen();
            isUserLogged = 1;
            screenWithLogout();
        }
        else if (!strcmp(commandAux, "logout") && isUserLogged)
        {
            logout();
            isUserLogged = 0;
            screenWithLogin();
        }
        else if (!strcmp(commandAux, "logout") && !isUserLogged)
        {
            errorLogout();
            usleep(1000000);
            screenWithLogin();
        }
        else if (strlen(commandAux))
        {
            printf(RED "\nCommand '%s' does not exist.\n\n" RESET, commandAux);
            usleep(600000);
            if (!isUserLogged)
                screenWithLogin();
            else
                screenWithLogout();
        }
    }
    return 0;
}