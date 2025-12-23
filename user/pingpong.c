#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int main(int argc, char *argv[])
{
    int pid;
    int pid1;
    char buf[2];

    if (argc != 1 ){
        fprintf(2,"Usage: pingpong\n");
        exit(1);
    }
    int p1[2];// 父→子
    int p2[2];// 子→父
    pipe(p2);
    pipe(p1);
    
    if (fork() == 0){ //子进程
        pid1 = getpid();
        close(p1[1]);
        read(p1[0], buf, 1);
        if (buf[0] == 'c'){
            fprintf(1,"%d: received ping\n", pid1);
            close(p2[0]);
            write(p2[1], &buf[0], 1);
            close(p2[1]);
        }
        
        close(p1[0]);
        exit(0);
    }
    else {  // 父进程
        pid = getpid();
        close(p1[0]);
        write(p1[1], "c", 1);
        close(p1[1]);
      
        close(p2[1]);
        read(p2[0], &buf[1], 1);
        close(p2[0]);

        wait(0);
        if (buf[1] == 'c')
            fprintf(1,"%d: received pong\n", pid);
    }

    exit(0);
}