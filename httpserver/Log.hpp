#pragma once
//�ṩ��־����

#include <iostream>
#include <time.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdarg.h>

// ��־�ȼ�
#define Info 0
#define Debug 1
#define Warning 2
#define Error 3
#define Fatal 4

#define SIZE 1024 // ��������С

// ��־�������ʽ
#define Screen 1    // �������ʾ��
#define Same_file 2 // �����ͬһ�ļ�
#define Diff_file 3 // ���յȼ��������ͬ�ļ�

#define Filename "log.txt"

class Log
{
public:
    Log()
    {
        _method = Screen; // Ĭ���������ʾ��
    }

    void output(int method) // ���������ʽ
    {
        _method = method;
    }

    std::string level2string(int level) // ��־�ȼ�ת���ַ���
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
        va_start(s, format); // sָ��ɱ����
        messagehandle(level, format, s);
    }

    void messagehandle(int level, const char *format, va_list s) // ������־�ַ���
    {
        time_t t = time(nullptr);         // ��ȡʱ���
        struct tm *ctime = localtime(&t); // ��ʱ���ת��Ϊʱ��
        char levelAndtime[SIZE];          // ��־�ȼ���ʱ�䲿��
        snprintf(levelAndtime, sizeof(levelAndtime), "[%s][%d-%d-%d %02d:%02d:%02d]", level2string(level).c_str(),
                 ctime->tm_year + 1900, ctime->tm_mon + 1, ctime->tm_mday, ctime->tm_hour, ctime->tm_min, ctime->tm_sec);

        char content[SIZE]; // �û��Զ�������ݲ���
        vsnprintf(content, sizeof(content), format, s);
        va_end(s);

        char message[SIZE * 2]; // �������в���
        snprintf(message, sizeof(message), "%s %s\n", levelAndtime, content);

        OutputLog(level, message); // �����Ϻ����־���
    }

    void OutputLog(int level, const std::string &logmessage)
    {
        switch (_method) // ���������ʽ���е���
        {
        case Screen: // �������ʾ��
            std::cout << logmessage;
            break;
        case Same_file: // �����ͬһ�ļ�
            SamefileOutput(Filename, logmessage);
            break;
        case Diff_file: // �������ͬ�ļ�
            DiffileOutput(level, logmessage);
            break;
        default:
            break;
        }
    }

    void SamefileOutput(const std::string &filename, const std::string &logmessage)
    {
        int fd = open(filename.c_str(), O_WRONLY | O_CREAT | O_APPEND, 0666); //���ļ�
        if(fd < 0) //��ʧ��
            return;
        write(fd, logmessage.c_str(), logmessage.size()); //д����־
        close(fd); //�ر��ļ�������
    }

    void DiffileOutput(int level, const std::string &logmessage)
    {
        std::string filename = Filename;
        filename += ".";
        filename += level2string(level); //������־�ȼ������ļ���
        SamefileOutput(filename, logmessage); //����SamefileOutput����
    }

    ~Log()
    {}

private:
    int _method; // �����ʽ
};



