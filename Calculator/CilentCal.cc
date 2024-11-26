// ���������-�ͻ���
#include "Socket.hpp"
#include "Protocol.hpp"
#include <ctime>
#include <unistd.h>

void Usage(std::string proc)
{
    std::cout << "Usage:\n\t" << proc << " port" << std::endl;
}

// ./clientcal ip port
int main(int argc, char *argv[])
{
    if (argc != 3)
    {
        Usage(argv[0]);
        exit(0);
    }

    std::string serverip = argv[1];
    uint16_t serverport = std::stoi(argv[2]);

    Sock sockfd;
    sockfd.Socket();
    bool flag = sockfd.Connect(serverip, serverport); // ��������
    if(!flag)
        return 1;

    int cnt = 1; //debug
    srand(time(0) ^ getpid());
    std::string ops = "+-*/%^&|";

    while (cnt <= 10) //����ʮ������
    {
        int x = rand() % 1000 + 1;
        usleep(1000);
        int y = rand() % 1000;
        usleep(1000);
        char op = ops[rand() % ops.size()];
        
        Request req(x, y, op); // �������
        req.Print();

        std::string content;
        req.Serialize(&content);
        std::string package = Encode(content);

        write(sockfd.sockfd_, package.c_str(), package.size());

        char buf[128];
        ssize_t n = read(sockfd.sockfd_, buf, sizeof(buf));
        if(n > 0) //��ȡ�ɹ�
        {
            buf[n] = 0;
            std::string res = buf;
            bool flag = Decode(res, &content); //ȥ����ͷ

            Response resp;
            flag = resp.Deserialize(content); //�����л�
            if(!flag)
                return 1;

            resp.Print();
        }
        else
            break;
        cnt++;
    }

    sockfd.Close();

    return 0;
}