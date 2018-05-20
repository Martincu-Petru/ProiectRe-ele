#include <stdio.h>

#define RESET "\x1B[0m"
#define RED "\x1B[31m"

void errorScreen()
{
    printf(RED "\nError while executing command. Not enough priviliges!\nPlease login first...\n\n" RESET);
}

void errorCommand()
{
    printf(RED "\nError while executing command. Too many parameters!\nPlease enter only the name of the file...\n\n" RESET);
}

void errorLogQuit()
{
    printf(RED "\nError while executing command. This is a non-parametrable command...\n\n" RESET);
}

void errorLogout()
{
    printf(RED "\nError while logging out command. There is no active user logged in...\n\n" RESET);
}

void errorConfig()
{
    printf(RED "\nError while reading from config file...\n\n" RESET);
}

void errorPipe()
{
    printf(RED "\nError while writing to pipe...\n\n" RESET);
}

void errorPipeRead()
{
    printf(RED "\nError while reading from pipe...\n\n" RESET);
}