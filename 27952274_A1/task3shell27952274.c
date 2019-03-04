#include <sys/stat.h>
#include <sys/types.h>
#include <stdio.h>
#include <unistd.h>
#include <dirent.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <sys/fcntl.h>

void listFile(char* dirName);
char *listPerm(char fileP);

int main (void)
{
    int quit = 0;
    printf("<program.background-color {white}>");
    listFile(".");  
    
    while (quit == 0){ 
        char options[5] = "dxvq";/*one char for each command*/
        char input[50];
        
        printf("<parent.push onclick=<put {d:}>,<putln {..}>>");

        scanf("%[^\n]%*c", input);
        
        pid_t pid;
        
        char opt1[] = {"xterm -e "};
        char opt2[] = {"xterm -e less "};
       
        if (input[0] == options[0])
        {  
            if (chdir(&input[2]) == 0){;
                printf("<program.clear>");
                listFile(".");
            }
            else{
                printf("<error.visible true>");
                printf("<error {You do not have permission}>");
                sleep(1);
                printf("<error.visible false>");
            }
        }
       
        if (input[0] == options[1])
        {

            pid = fork();
           
            if (pid == 0){
                system(strcat(opt1, &input[2]));          
            }
        }    


        if (input[0] == options[2])
        {
            FILE *fp = fopen(&input[2],"r");
            fseek(fp, 0L, SEEK_END);
            int q = ftell(fp);
            
            if (q < 5000){
                system(strcat(opt2, &input[2]));
            }
            else{
                printf("<error.visible true>");
                printf("<error {File size too large}>");
                sleep(1);
                printf("<error.visible false>");
            }
        }
      
    }
           
    return 0;
}


void listFile(char* dirName){

    DIR *dir;
    struct dirent *dit;
    struct stat filestat;
    char filePerm[10];/* used to store the octal number of file permission*/

    dir = opendir(dirName);

    char v[4] = {"47"};/*this strin is used to determine the file type 4 for directory,7 for executable files*/
    
    char type[15];
    char cwd[PATH_MAX];
    char* hidden = ".";/*to compare against hidden files*/
    
    getcwd(cwd, sizeof(cwd));
    printf("<parent:button color={black} {</#x2190>} {<span bold=true color={black} {Go Back}>}>");
    printf("<  error:box color={red} visible=false>");
    printf("</p>");
    printf("<info:box color={black} visible=false>");

    printf("</p>");
    printf("<box color={black} {Current directory: %s </p>} visible=true>",cwd);
    
    while ((dit = readdir(dir)) != NULL){
        printf("</p>");
        stat(dit->d_name,&filestat);
        sprintf(filePerm,"%o",filestat.st_mode);
        
        char opt1[] = {"stat -c '%A' "};
        char dest[] = {" >> perm"};
       
        strcat(opt1,dit->d_name);
        system(strcat(opt1,dest));

        char bufPerm[1024];
        read(open("perm",O_RDONLY), bufPerm, sizeof(bufPerm));
        

        if (strcmp(dit->d_name, (".")) == 0 || strcmp(dit->d_name, ("..")) == 0 ||  (*dit->d_name == *hidden))
        {
            continue;
        }
        if ( strlen(filePerm) == 5){                      
            if (filePerm[0] == v[0]){
                printf("<dir:button color={white} onclick=<put {d:}>,<putln {%s}>", dit->d_name);
                printf("{<b:box size=25 color={orange} {</#x1F4C2>} {");
            }
        }
        else if (filePerm[3] == v[1]){
            FILE *fp = fopen(dit->d_name,"r");
            fseek(fp, 0L, SEEK_END);
            
            printf("<exe:button color={white} onclick=<put {x:}>,<putln {%s}>oncontextclick=<info {size is:%ld kB, permission:%s}>,<info.visible true>", dit->d_name,ftell(fp)/1000,bufPerm);
            printf("{<box size=25 color={blue} {</#x1F5D4>} {");
        }
        else
        {
            FILE *fp = fopen(dit->d_name,"r");
            fseek(fp, 0L, SEEK_END);
            printf("<txt:button color={white} onclick=<put {v:}>,<putln {%s}>oncontextclick=<info {size is:%ld kB, permission:%s}>,<info.visible true>",dit->d_name,ftell(fp)/1000,bufPerm);
            printf("{<box size=25 color={green} {</#x1F5CE>} {");
        }
        remove("perm");
        printf("<s:span color={black} {%s}>}>}>", dit->d_name);
    }

}


