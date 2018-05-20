#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include <sys/types.h>
#include <dirent.h>
#include <time.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/wait.h>



void processDirectory(const char *path, char fileName[], int FD, int *findAtLeast)
{
    *findAtLeast = 0;
    fileName[strcspn(fileName, "\n")] = '\0';
    DIR *directory;
    struct dirent *enterDirectory;
    char newPath[100];
    if (NULL == (directory = opendir(path)))
    {
        perror("Error at opening directory: ");
        exit(-1);
    }
    if(-1 == dup2(1, FD))
    {
        perror("Error while trying to redirect output to FIFO...");
        exit(-1);
    }
    while ((enterDirectory = readdir(directory)) != NULL)
    {
        if (enterDirectory->d_type & DT_REG) 
        {
            if (strcmp(enterDirectory->d_name, fileName) == 0)
            {
                *findAtLeast = 1;
                for (int i = 0; i <= 99; i++)
                    newPath[i] = '\0';
                strcpy(newPath, path);
                newPath[strlen(newPath)] = '/';
                strcpy(newPath + strlen(newPath), enterDirectory->d_name);
                printf("\n%s\n", newPath);
                struct stat statbuf;
                if (-1 == stat(newPath, &statbuf))
                {
                    perror("Error at finding stat: \n");
                    exit(-1);
                }
                printf("\nLast status change:       %s", ctime(&statbuf.st_ctime));
                printf("Last file access:         %s", ctime(&statbuf.st_atime));
                printf("Last file modification:   %s", ctime(&statbuf.st_mtime));
                printf("File size:                %lld bytes\n", (long long)statbuf.st_size);
                printf("File Permissions: \t");
                if(statbuf.st_mode & S_IRUSR)
                printf("  r");printf("-");
                if(statbuf.st_mode & S_IWUSR)
                printf("w");printf("-");
                if(statbuf.st_mode & S_IXUSR)
                printf("x");printf("-");
                if(statbuf.st_mode & S_IRGRP)
                printf("r");printf("-");
                if(statbuf.st_mode & S_IWGRP)
                printf("w");printf("-");
                if(statbuf.st_mode & S_IXGRP)
                printf("x");printf("-");
                if(statbuf.st_mode & S_IROTH)
                printf("r");printf("-");
                if(statbuf.st_mode & S_IWOTH)
                printf("w");printf("-");
                if(statbuf.st_mode & S_IXOTH)
                printf("x");printf("-");
                printf("\n");
                printf("I-node number:            %ld\n", (long)statbuf.st_ino);
                printf("File type:                ");
                switch (statbuf.st_mode & S_IFMT) {
                    case S_IFBLK:  
                        printf("block device\n");            
                        break;
                    case S_IFCHR:  
                        printf("character device\n");        
                        break;
                    case S_IFDIR:  
                        printf("directory\n");               
                        break;
                    case S_IFIFO:  
                        printf("FIFO/pipe\n");               
                        break;
                    case S_IFLNK:  
                        printf("symlink\n");                 
                        break;
                    case S_IFREG:  
                        printf("regular file\n");            
                        break;
                    case S_IFSOCK: 
                        printf("socket\n");                  
                        break;
                    default:       
                        printf("unknown?\n");                
                        break;
                    }
                    printf("Mode:                     %lo (octal)\n", (unsigned long) statbuf.st_mode);
                    printf("Link count:               %ld\n", (long) statbuf.st_nlink);
                    printf("Ownership:                UID=%ld   GID=%ld\n", (long) statbuf.st_uid, (long) statbuf.st_gid);
                    printf("Preferred I/O block size: %ld bytes\n", (long) statbuf.st_blksize); 
                    printf("File size:                %lld bytes\n", (long long) statbuf.st_size); 
                    printf("Blocks allocated:         %lld\n", (long long) statbuf.st_blocks);
            }
        }
        if (enterDirectory->d_type & DT_DIR)
        {
            if (strcmp(enterDirectory->d_name, "..") != 0 && strcmp(enterDirectory->d_name, ".") != 0)
            {
                for (int i = 0; i <= 99; i++)
                    newPath[i] = '\0';
                strcpy(newPath, path);
                newPath[strlen(newPath)] = '/';
                strcpy(newPath + strlen(newPath), enterDirectory->d_name);
                processDirectory(newPath, fileName, FD, findAtLeast);
            }
        }
    }
}

void processDirectoryNonRecursive(char fileName[])
{
    pid_t PID;
    struct stat statbuf;
    int socket[2];

    if (socketpair(AF_UNIX, SOCK_STREAM, 0, socket) < 0)
    {
        perror("Error while establishing the socketpair: ");
        exit(-1);
    }

    if (-1 == (PID = fork()))
    {
        perror("Error while executing fork: ");
        exit(-1);
    }
    if (PID == 0)
    {
        close(socket[0]);
        fileName[strcspn(fileName, "\n")] = '\0';
        char path[256];
        path[0] = '.'; path[1] = '/';
        for (int i = 0; i < strlen(fileName); i++)
            path[i + 2] = fileName[i];
        path[strlen(path)] = '\0';
        dup2(1, socket[1]);
        if (-1 == stat(path, &statbuf))
        {
            perror("Error at finding stat");
            exit(-1);
        }
        printf("\nLast status change:       %s", ctime(&statbuf.st_ctime));
        printf("Last file access:         %s", ctime(&statbuf.st_atime));
        printf("Last file modification:   %s", ctime(&statbuf.st_mtime));
        printf("File size:                %lld bytes\n", (long long)statbuf.st_size);
        printf("File Permissions: \t");
        if(statbuf.st_mode & S_IRUSR)
        printf("  r");printf("-");
        if(statbuf.st_mode & S_IWUSR)
        printf("w");printf("-");
        if(statbuf.st_mode & S_IXUSR)
        printf("x");printf("-");
        if(statbuf.st_mode & S_IRGRP)
        printf("r");printf("-");
        if(statbuf.st_mode & S_IWGRP)
        printf("w");printf("-");
        if(statbuf.st_mode & S_IXGRP)
        printf("x");printf("-");
        if(statbuf.st_mode & S_IROTH)
        printf("r");printf("-");
        if(statbuf.st_mode & S_IWOTH)
        printf("w");printf("-");
        if(statbuf.st_mode & S_IXOTH)
        printf("x");printf("-");
        printf("\n");
        printf("I-node number:            %ld\n", (long)statbuf.st_ino);
        printf("File type:                ");
        switch (statbuf.st_mode & S_IFMT) {
        case S_IFBLK:  
            printf("block device\n");            
            break;
        case S_IFCHR:  
            printf("character device\n");        
            break;
        case S_IFDIR:  
            printf("directory\n");               
            break;
        case S_IFIFO:  
            printf("FIFO/pipe\n");               
            break;
        case S_IFLNK:  
            printf("symlink\n");                 
            break;
        case S_IFREG:  
            printf("regular file\n");            
            break;
        case S_IFSOCK: 
            printf("socket\n");                  
            break;
        default:       
            printf("unknown?\n");                
            break;
        }
        printf("Mode:                     %lo (octal)\n", (unsigned long) statbuf.st_mode);
        printf("Link count:               %ld\n", (long) statbuf.st_nlink);
        printf("Ownership:                UID=%ld   GID=%ld\n", (long) statbuf.st_uid, (long) statbuf.st_gid);
        printf("Preferred I/O block size: %ld bytes\n", (long) statbuf.st_blksize); 
        printf("File size:                %lld bytes\n", (long long) statbuf.st_size); 
        printf("Blocks allocated:         %lld\n", (long long) statbuf.st_blocks);
        fflush(stdout);
        close(socket[1]);
        exit(0);
    }
    if (PID != 0)
    {
        char character;
        wait(NULL);
        close(socket[1]);
        while (read(socket[0], &character, sizeof(character)) > 0)
            printf("%c", character);
        if(-1 == read(socket[0], &character, sizeof(character)))
            perror("Error while reading from socket...");
        close(socket[1]);
    }
}