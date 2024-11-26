#pragma once
// Tcp服务端

#include "Log.hpp"
#include "Socket.hpp"
#include "Calculator.hpp"
#include <signal.h>
#include <functional>
#include <string>

using func_t = std::function<std::string(std::string &)>;

class TcpServer
{
public:
    TcpServer(uint16_t port, func_t callback)
        :port_(port), callback_(callback)
    {}

    ~TcpServer()
    {}

public:
    bool Init() // 初始化服务器
    {
        listensock_.Socket();
        listensock_.Bind(port_);
        listensock_.Listen();
        log(Info, "server init success");
        return true;
    }

    void Start() // 启动服务器
    {
        signal(SIGCHLD, SIG_IGN);
        signal(SIGPIPE, SIG_IGN);
        while (true)
        {
            std::string clientip;
            uint16_t clientport;
            int sockfd = listensock_.Accept(&clientip, &clientport); // 接收连接
            if (sockfd < 0)
                continue;
            log(Info, "server accept a new link, ip: %s, port: %d", clientip.c_str(), clientport);

            // 提供服务
            if (fork() == 0)
            {
                listensock_.Close();

                std::string inbuffer_stream;
                while (true)
                {
                    char buf[128];
                    ssize_t n = read(sockfd, buf, sizeof(buf));
                    if (n > 0)
                    {
                        buf[n] = 0;
                        inbuffer_stream += buf;

                        // 开始计算
                        std::string msg = callback_(inbuffer_stream);
                        if(msg.empty()) //进行序列化或反序列化时发送错误，返回空串
                            continue;

                        write(sockfd, msg.c_str(), msg.size());
                    }
                    else
                        break;
                }
                exit(0);
            }
            close(sockfd);
        }
    }

public:
    uint16_t port_; //服务器端口号
    Sock listensock_; //监听套接字
    func_t callback_; //回调函数-Calculate
};