#include <sys/stat.h>
#include <sys/types.h>
#include <stdio.h>
#include <unistd.h>
#include <dirent.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>

void listFile(char* dirName);

int main (void)
{
    listFile(".");    
    char options[5] = "dxvq";/*one char for each command*/
    char input[50];
    
    
    int quit = 0;
    char opt1[] = {"xterm -e "};
    char opt2[] = {"xterm -e less "};

    while (quit == 0){
        pid_t pid;
        scanf("%[^\n]%*c", input);
        if (input[0] == options[0]){
  
            if (chdir(&input[2]) == 0){;
                listFile(".");
            }
            else{
                printf("sorry you do not have permission to enter this directory");
            }
        }
       
        if (input[0] == options[1]){

            pid = fork();
            if (pid == 0){
                system(strcat(opt1, &input[2]));          
            }
            else{
                listFile(".");   
            }
        }    

        if (input[0] == options[2]){
            pid = fork();
            
            if (pid == 0){
                FILE *fp = fopen(&input[2],"r");
                fseek(fp, 0L, SEEK_END);

                if (ftell(fp) < 5000){
                    system(strcat(opt2, &input[2]));
                }
                else{
                    printf("File size is too large");
                }
            }
            else{
                listFile(".");
            }
            
        }
        if (input[0] == options[3]){
            quit = 1;
        }
    }
    
    return 0;
}


void listFile(char* dirName){
    /* this function takes the directory name as an argumnent and prints out all the files in that directory*/
    DIR *dir;
    struct dirent *dit;
    struct stat filestat;
    char filePerm[10];/* used to store the octal number of file permission*/

    dir = opendir(dirName);

    char v[4] = {"47"};/*this strin is used to determine the file type 4 for directory,7 for executable files*/
    char type[15];
    char cwd[PATH_MAX];
    char* hidden = ".";/*to compare against hidden files*/
    char test[10];
    getcwd(cwd, sizeof(cwd));
    printf("Current directory:  %s\n",cwd);
    printf("***Directory listing follows***\n");

    while ((dit = readdir(dir)) != NULL){
  
        stat(dit->d_name,&filestat);
        sprintf(filePerm,"%o",filestat.st_mode);
        
        if (strcmp(dit->d_name, (".")) == 0 || strcmp(dit->d_name, ("..")) == 0 ||  (*dit->d_name == *hidden))
        {
        continue;
        }
        /*if the length of octal permission code is 5, means that it is a directory*/
        if ( strlen(filePerm) == 5){            
                                  
            if (filePerm[0] == v[0]){
                strcpy(type,"Directory");
            }
        }
        else if (filePerm[3] == v[1]){
            strcpy(type,"Executable");
        }
        else{
            strcpy(type,"File");
        }
        printf("%-12s:%-12s\n",type,dit->d_name);
    }
    printf("\n");

}


