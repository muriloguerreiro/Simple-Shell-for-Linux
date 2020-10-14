// Simple Shell for Linux
// Author: Murilo Guerreiro Badoco

// To compile type: gcc shell.c -lreadline -o shell
// To run type: ./shell 

#include<stdio.h> 
#include<string.h> 
#include<stdlib.h> 
#include<unistd.h> 
#include<sys/types.h> 
#include<sys/wait.h> 
#include<readline/readline.h> 
#include<readline/history.h>

int getCommand(char* commandLine) {
    char* buff;

    buff = readline("$ ");
    if(strlen(buff) !=0) {
        add_history(buff);
        strcpy(commandLine, buff);
        return 0;
    }
    else return 1;
}

void printPath() {
    char buffer[2048];
    getcwd(buffer, 2048);
    printf("~%s", buffer);
}

void parseCommands(char* commands, char** parsed){
    for(int i=0 ; i<20 ; i++) {
        parsed[i] = strsep(&commands, " ");

        if(parsed[i] == NULL)
            break;

        if(strlen(parsed[i]) == 0)
            i--;
    }
}

void execFunction(char** parsed) {
    pid_t pid, wpid;
    int status;

    pid = fork();
    if(pid == -1) {
        printf("Forking just failed!");
        return;
    }

    else if(pid == 0) {
        if(execvp(parsed[0], parsed) <0) printf("Command not found\n");
        exit(0);
    }

    else {
        do {
            wpid = waitpid(pid, &status, WUNTRACED);
        } while (!WIFEXITED(status) && !WIFSIGNALED(status));
        return;
    }
}

int changeDirectory(char** parsed) {
    if(strcmp(parsed[0], "cd") == 0) {
        chdir(parsed[1]);
        return 1;
    }
    return 0;
}

int processStr(char* commands, char** parsed) {
    parseCommands(commands, parsed);

    if(changeDirectory(parsed))
        return 0;
    else
        return 1;
}

int main() {
    char input[500], *parsedArgs[20];
    int execFlag;

    while(1) {
        printPath();

        if(getCommand(input)) continue;

        execFlag = processStr(input, parsedArgs);

        if(execFlag == 1)
            execFunction(parsedArgs);
    }

    return 0;
}