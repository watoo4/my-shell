#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>
#define MAXARG 8

void run_cmd(char cmd[], int isbg) {
    char *args[MAXARG];
    char *s, *save;
    static const char delim[]=" \t\n";
    int argn;
    int pid, status;
    argn=0;

    s=strtok_r(cmd, delim, &save);

    if(s==NULL) return;
    while(s) {
        if(*s=='&') {
            break;
        }
        args[argn++] = s;
        s = strtok_r(NULL,delim,&save);
    }
    args[argn] = (char *)0;

    if(!strcmp(args[0], "quit"))
        exit(0);
    if((pid=fork())==-1)
        perror("fork failed");
    else if(pid!=0) {
        if(isbg==0)
            pid = wait(&status);
        else {
            printf("[1] %d\n",getpid());
            waitpid(pid,&status,WNOHANG);
        }
    }
        else {
        execvp(args[0],args);
        fprintf(stderr, "fail to run %s\n",args[0]);
        exit(0);
    }
}

int main() {
    int isbg=1;
    char *bgptr;
    char buf[256];
    char *args[MAXARG];
    char *cmd;
    char *save;
    static const char delim_cmd[]=";\n";

    while(1) {
        printf("[202120992@shell] ");
        fgets(buf,256,stdin);
        cmd=strtok_r(buf,delim_cmd, &save);
        if(strchr(cmd,'&')==NULL) {
            isbg=0;
        }

        while(cmd) {
            run_cmd(cmd, isbg);
            cmd = strtok_r(NULL, delim_cmd, &save);
        }

        sleep(1);
    }
    exit(0);
}