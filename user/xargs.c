#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fs.h"

// 执行命令
void run(char *pram, char **args)
{
    if(fork() == 0) { // child exec
		exec(pram, args);
		exit(0);
	}
    return;
}

int main(int argc, char *argv[])
{
    char buf[128];  // 读入时使用的内存池
    char *p_in = buf; // 读取标准输入参数的开始指针
    char *p_out = buf; // 标准输入参数的准备输入xargs的参数开始
    char *argsbuf[128]; // 全部参数，字符串指针数组，包含 argv 传进来的参数和 stdin 读入的参数
    char **args = argsbuf;
    for(int i=1; i< argc; i++){ // 读取argv的参数
        *args = argv[i];
        args++;
    }

    char **in_argv = args;  // in_argv继承args的位置，继续传入标准输入的参数
    while(read(0, p_in, 1) != 0 ){
        
        if(*p_in == ' ' || *p_in == '\n'){
            
            *p_in = '\0'; // 将空格替换为 \0 分割开各个参数
            *(in_argv++) = p_out; // 将buf的参数赋值给in_argv指向argsbuf后续位置
            p_out = p_in + 1;

            if(*p_in == '\n') {
				// 读入一行完成
				*in_argv = 0; // 参数列表末尾用 null 标识列表结束
                in_argv = args; // 重置读入参数指针，准备读入下一行
				run(argv[1], argsbuf); // 执行最后一行指令
                wait(0);
				
			}

        }
        p_in++;
    }

    if (in_argv != args){
        *p_in = '\0';
        *(in_argv++) = p_out;
        *in_argv = 0;
        run(argv[1], argsbuf);
        wait(0);
    }
    
    exit(0);
}