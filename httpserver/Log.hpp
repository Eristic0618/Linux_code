#pragma once
//提供日志功能

#include <iostream>
#include <time.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdarg.h>

// 日志等级
#define Info 0
#define Debug 1
#define Warning 2
#define Error 3
#define Fatal 4

#define SIZE 1024 // 缓冲区大小

// 日志的输出方式
#define Screen 1    // 输出到显示器
#define Same_file 2 // 输出到同一文件
#define Diff_file 3 // 按照等级输出到不同文件

#define Filename "log.txt"

class Log
{
public:
    Log()
    {
        _method = Screen; // 默认输出到显示器
    }

    void output(int method) // 更改输出方式
    {
        _method = method;
    }

    std::string level2string(int level) // 日志等级转换字符串
    {
        switch (level)
        {
        case Info:
            return "Info";
        case Debug:
            return "Debug";
        case Warning:
            return "Warning";
        case Error:
            return "Error";
        case Fatal:
            return "Fatal";
        default:
            return "None";
        }
    }

    void operator()(int level, const char *format, ...)
    {
        va_list s;
        va_start(s, format); // s指向可变参数
        messagehandle(level, format, s);
    }

    void messagehandle(int level, const char *format, va_list s) // 整合日志字符串
    {
        time_t t = time(nullptr);         // 获取时间戳
        struct tm *ctime = localtime(&t); // 将时间戳转换为时间
        char levelAndtime[SIZE];          // 日志等级和时间部分
        snprintf(levelAndtime, sizeof(levelAndtime), "[%s][%d-%d-%d %02d:%02d:%02d]", level2string(level).c_str(),
                 ctime->tm_year + 1900, ctime->tm_mon + 1, ctime->tm_mday, ctime->tm_hour, ctime->tm_min, ctime->tm_sec);

        char content[SIZE]; // 用户自定义的内容部分
        vsnprintf(content, sizeof(content), format, s);
        va_end(s);

        char message[SIZE * 2]; // 整合所有部分
        snprintf(message, sizeof(message), "%s %s\n", levelAndtime, content);

        OutputLog(level, message); // 将整合后的日志输出
    }

    void OutputLog(int level, const std::string &logmessage)
    {
        switch (_method) // 根据输出方式进行调整
        {
        case Screen: // 输出到显示器
            std::cout << logmessage;
            break;
        case Same_file: // 输出到同一文件
            SamefileOutput(Filename, logmessage);
            break;
        case Diff_file: // 输出到不同文件
            DiffileOutput(level, logmessage);
            break;
        default:
            break;
        }
    }

    void SamefileOutput(const std::string &filename, const std::string &logmessage)
    {
        int fd = open(filename.c_str(), O_WRONLY | O_CREAT | O_APPEND, 0666); //打开文件
        if(fd < 0) //打开失败
            return;
        write(fd, logmessage.c_str(), logmessage.size()); //写入日志
        close(fd); //关闭文件描述符
    }

    void DiffileOutput(int level, const std::string &logmessage)
    {
        std::string filename = Filename;
        filename += ".";
        filename += level2string(level); //根据日志等级调整文件名
        SamefileOutput(filename, logmessage); //复用SamefileOutput函数
    }

    ~Log()
    {}

private:
    int _method; // 输出方式
};



