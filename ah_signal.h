#ifndef ahsignal_h
#define ahsignal_h
#include <signal.h>
#include <stdio.h>
#include <unistd.h>
#include <errno.h>

void ah_signal(int _signum,void (*_act)(int)){
	signal(_signum,_act);
}

void set_group(char *gname){
    FILE *ifp;
    char fname[50];
    sprintf(fname,"/usr/share/.%s",gname);
    ifp = fopen(fname, "a");
    fprintf(ifp, "%d\n",getpid());
    fclose(ifp);
}

int send_signal(int _signal,char *gname){
    FILE *ifp;
    char fname[50];
    sprintf(fname,"/usr/share/.%s",gname);
    ifp = fopen(fname,"r");
    int _pid;
    while (!feof(ifp)){  
      fscanf (ifp, "%d", &_pid);
      kill(_pid,_signal); 
    }
    fclose(ifp);
}

void clean_group(char *gname){
    char fname[50];
    sprintf(fname,"/usr/share/.%s",gname);
    remove(fname);
}
#endif