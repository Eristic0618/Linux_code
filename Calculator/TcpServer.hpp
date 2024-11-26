#pragma once
// Tcp�����

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
    bool Init() // ��ʼ��������
    {
        listensock_.Socket();
        listensock_.Bind(port_);
        listensock_.Listen();
        log(Info, "server init success");
        return true;
    }

    void Start() // ����������
    {
        signal(SIGCHLD, SIG_IGN);
        signal(SIGPIPE, SIG_IGN);
        while (true)
        {
            std::string clientip;
            uint16_t clientport;
            int sockfd = listensock_.Accept(&clientip, &clientport); // ��������
            if (sockfd < 0)
                continue;
            log(Info, "server accept a new link, ip: %s, port: %d", clientip.c_str(), clientport);

            // �ṩ����
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

                        // ��ʼ����
                        std::string msg = callback_(inbuffer_stream);
                        if(msg.empty()) //�������л������л�ʱ���ʹ��󣬷��ؿմ�
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
    uint16_t port_; //�������˿ں�
    Sock listensock_; //�����׽���
    func_t callback_; //�ص�����-Calculate
};