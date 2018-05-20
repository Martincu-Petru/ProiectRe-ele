#include <string.h>
#include <stdio.h>

int processCommand(char commandAux[], char command[], char fileName[], int *loginQuitLogout)
{
    char * parametru;
    *loginQuitLogout = 0;
    int numberOfParameters = -1;
    strcpy(commandAux, command);
        while (commandAux[0] == ' ')
            strcpy(commandAux, commandAux + 1);
        commandAux[strlen(commandAux)] = '\0';
    parametru = strtok(commandAux, " ");
    if(!strcmp(parametru, "login") || !strcmp(parametru, "quit") || !strcmp(parametru, "logout"))
        *loginQuitLogout = 1;
    
    while(parametru != NULL)
    {
        numberOfParameters++;
        if(numberOfParameters == 1)
            {
                if(*loginQuitLogout)
                    return -1;
                strcpy(fileName, parametru);
            }
        if(numberOfParameters == 2)
            return -1;
        parametru = strtok(NULL, " ");
    }
    commandAux[strcspn(commandAux, "\n")] = '\0';
    return 0;
}