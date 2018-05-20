#include <stdio.h>
#include <stdlib.h>

#define RESET "\x1B[0m"
#define RED "\x1B[31m"
#define GRN "\x1B[32m"
#define BLU "\x1B[34m"
#define CYN "\x1B[36m"

void screenWithLogin()
{
    system("clear");
    printf(CYN "MAIN SCREEN\n\n" RESET);
    printf(GRN "Available commands:" RESET BLU " login " RESET "|" RED " quit\n\n" RESET);
}

void screenWithLogout()
{
    system("clear");
    printf(CYN "MAIN SCREEN\n\n" RESET);
    printf(GRN "Available commands:" RESET BLU " logout " RESET "|" BLU " myfind " RESET "|" BLU " mystat " RESET "|" RED " quit\n\n" RESET);
}

void commandsScreen()
{
    printf(GRN "\nAvailable commands:" RESET BLU " logout " RESET "|" BLU " myfind " RESET "|" BLU " mystat " RESET "|" RED " quit\n\n" RESET);
    fflush(stdout);
}