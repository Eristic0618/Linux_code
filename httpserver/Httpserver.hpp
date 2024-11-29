#include "Log.hpp"
#include "Socket.hpp"
#include <pthread.h>

class PthreadData
{
public:
    PthreadData(int sockfd)
        : sockfd_(sockfd)
    {
    }

public:
    int sockfd_;
};

class HttpServer
{
public:
    HttpServer(uint16_t port)
        : port_(port)
    {
    }

    ~HttpServer()
    {
    }

public:
    void Start()
    {
        listensock_.Socket();
        listensock_.Bind(port_);
        listensock_.Listen();

        while (true)
        {
            int sockfd = accept(listensock_.sockfd_, 0, 0); // ��ȡ����
            if (sockfd < 0)
                continue;

            PthreadData *pd = new PthreadData(sockfd);
            pthread_t serv;
            pthread_create(&serv, 0, handler, pd); // �����߳�
        }
    }

    static void handlerHelper(int sockfd)
    {
        char inbuf[128];
        ssize_t n = recv(sockfd, inbuf, sizeof(inbuf) - 1, 0); //��ȡ����
        if (n > 0)
        {
            inbuf[n] = 0;
            std::string request = inbuf;

            //Ӳ���빹����Ӧ
            std::string response_line = "HTTP/1.0 200 OK\r\n";
            std::string response_content = "Content_Length:";
            response_content += std::to_string(request.size());
            std::string blank_line = "\r\n";
            std::string content = "hello world\r\n";

            std::string response = response_line;
            response += response_content;
            response += blank_line;
            response += content;
            

            send(sockfd, response.c_str(), response.size(), 0);
        }
    }

    static void *handler(void *arg)
    {
        pthread_detach(pthread_self()); // �̷߳���

        PthreadData *pd = static_cast<PthreadData *>(arg);
        // �������󲢹�����Ӧ
        handlerHelper(pd->sockfd_);
        close(pd->sockfd_);
        exit(0);
    }

public:
    Sock listensock_;
    uint16_t port_;
};