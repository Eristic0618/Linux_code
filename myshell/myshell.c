#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <assert.h>

#define LABLE "#"
#define LINE_SIZE 1024
#define QUIT 0
#define ARGC_SIZE 32
#define EXIT_CODE 4

char commandline[LINE_SIZE];
char* argv[ARGC_SIZE];
int argc;
int last_code;
char pwd[LINE_SIZE];
char myenv[LINE_SIZE];

const char* gethostname() //获取主机名
{
    return getenv("HOSTNAME");
}

const char* getusername() //获取用户名
{
    return getenv("USER");
}

void getpwd() //获取工作路径
{ 
    getcwd(pwd, sizeof(pwd));
}

void printinfo() //打印命令行信息
{
    getpwd();
    printf("[%s@%s %s]"LABLE" ", getusername(), gethostname(), pwd);
}

void getcommand() //获取输入的命令
{
    char* s = fgets(commandline, sizeof(commandline), stdin);
    assert(s != NULL);
    (void)s;
    commandline[strlen(commandline)-1] = '\0'; //去掉\n
}

int parsecommand() //解析命令:将长串字符串切割为一个个选项
{
    int i = 0;
    argv[i++] = strtok(commandline, " \t");
    while(argv[i++] = strtok(NULL, " \t"));
    return i-1;
}

int ExecuteBuiltinCommands() //执行内建命令
{
    if(argc == 2 && strcmp(argv[0], "cd") == 0) //cd命令
    {
        chdir(argv[1]);
        getpwd();
        sprintf(getenv("PWD"), "%s", pwd);
        return 1;
    }
    else if(argc == 2 && strcmp(argv[0], "export") == 0) //export命令
    {
        strcpy(myenv, argv[1]);
        putenv(myenv);
        return 1;
    }
    else if(argc == 2 && strcmp(argv[0], "echo") == 0) //echo命令
    {
        if(strcmp(argv[1], "$?") == 0) //打印上一次的退出码
        {
            printf("%d\n", last_code);
            last_code = 0;
        }
        else if(*argv[1] == '$') //判断是否要输出环境变量
        {
            char* s = getenv(argv[1]+1);
            if(s) printf("%s\n", s);
        }
        else 
            printf("%s\n", argv[1]);
        return 1;
    }
    //...
    if(strcmp(argv[0], "ls") == 0) //特殊处理
    {
        argv[argc++] = "--color"; //给不同文件加上颜色
        argv[argc] = NULL;
    }
    return 0;
}

void ExecuteRegularCommands() //执行普通命令
{
    pid_t id = fork(); //创建子进程执行命令
    if(id < 0)
    {
        perror("fork error");
        return;
    }
    else if(id == 0)
    {
        //程序替换
        execvp(argv[0], argv);
        exit(EXIT_CODE);
    }
    else 
    {
        int status = 0;
        pid_t ret = waitpid(id, &status, 0); //阻塞式等待
        if(ret == id) //等待成功
        {
            last_code = WEXITSTATUS(status); //保留退出码       
        }
    }
}

int main()
{
    while(!QUIT)
    {
        printinfo(); //打印命令行信息
        getcommand(); //获取输入的命令
        argc = parsecommand(); //解析命令
        if(argc == 0) continue;
        int flag = ExecuteBuiltinCommands(); //判断是否为内建命令
        if(!flag) ExecuteRegularCommands(); //执行普通命令
    }
    return 0;
}
