#pragma once
//提供socket相关接口

#include <iostream>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <cstring>
#include <unistd.h>
#include <string>
#include "Log.hpp"

Log log;

enum
{
    Socket_Err = 2,
    Bind_Err,
    Listen_Err
};

class Sock
{
public:
    Sock()
    {}

    ~Sock()
    {}

public:
    void Socket() // 创建套接字
    {
        sockfd_ = socket(AF_INET, SOCK_STREAM, 0); // 创建监听套接字
        if (sockfd_ < 0)
        {
            log(Fatal, "socket create failed, errno: %d, errmsg: %s", errno, strerror(errno));
            exit(Socket_Err);
        }
        log(Info, "socket create success");
    }

    void Bind(uint16_t port) // 绑定套接字
    {
        struct sockaddr_in local;
        memset(&local, 0, sizeof(local));

        local.sin_family = AF_INET;
        local.sin_port = htons(port);
        local.sin_addr.s_addr = INADDR_ANY;

        socklen_t len = sizeof(local);
        if (bind(sockfd_, (struct sockaddr *)&local, len))
        {
            log(Fatal, "bind failed, errno: %d, errmsg: %s", errno, strerror(errno));
            exit(Bind_Err);
        }
        log(Info, "bind success");
    }

    void Listen() // 监听套接字
    {
        if (listen(sockfd_, 0) < 0)
        {
            log(Fatal, "listen failed, errno: %d, errmsg: %s", errno, strerror(errno));
            exit(Listen_Err);
        }
        log(Info, "listen success");
    }

    int Accept(std::string *clientip, uint16_t *clientport) // 接收连接
    {
        struct sockaddr_in peer;
        socklen_t len = sizeof(peer);
        int newfd = accept(sockfd_, (struct sockaddr *)&peer, &len);
        if(newfd < 0)
        {
            log(Warning, "accept failed, errno: %d, errmsg: %s", errno, strerror(errno));
            return -1;
        }

        char buf[64];
        inet_ntop(AF_INET, &(peer.sin_addr), buf, sizeof(buf));
        *clientip = buf;
        *clientport = ntohs(peer.sin_port);

        return newfd;
    }

    bool Connect(std::string& ip, uint16_t port) // 客户端发起连接
    {
        struct sockaddr_in server;
        memset(&server, 0, sizeof(server));
        server.sin_family = AF_INET;
        server.sin_port = htons(port);
        inet_pton(AF_INET, ip.c_str(), &(server.sin_addr));

        int n = connect(sockfd_, (struct sockaddr*)&server, sizeof(server));
        if(n < 0)
        {
            std::cerr << "connect failed" << std::endl;
            return false;
        }
        return true;
    }

    void Close() //关闭套接字
    {
        close(sockfd_);
    }

public:
    int sockfd_;
};