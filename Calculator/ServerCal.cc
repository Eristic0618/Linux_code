//���������-�����
#include "Socket.hpp"
#include "TcpServer.hpp"
#include "Protocol.hpp"
#include "Calculator.hpp"
#include "Log.hpp"

void request_test()
{
    Request req(1234562, 4512316, '+');
    std::string s1;
    req.Serialize(&s1); //���л�

    s1 = Encode(s1); //��ӱ�ͷ
    std::cout << s1 << std::endl;

    std::string s2;
    bool r = Decode(s1, &s2); //ȥ����ͷ
    std::cout << r << " " << s2 << std::endl;

    Request temp;
    temp.Deserialize(s2); //�����л�
    std::cout << temp.x_ << temp.op_ << temp.y_ << std::endl;
}

void response_test()
{
    Response resp(1000, 0);
    std::string content;
    resp.Serialize(&content);
    std::cout << content << std::endl;

    std::string package = Encode(content);
    std::cout << package << std::endl;

    Response temp;
    temp.Deserialize(content);
    std::cout << temp.result_ << " " << temp.code_ << std::endl;
}

void Usage(std::string proc)
{
    std::cout << "Usage:\n\t" << proc << " port" << std::endl;
}

// ./servercal port
int main(int argc, char* argv[])
{
    // request_test();
    // response_test();

    if(argc != 2)
    {
        Usage(argv[0]);
        exit(0);
    }

    uint16_t port = std::stoi(argv[1]);
    TcpServer *tsvr = new TcpServer(port, std::bind(&Calculator::Calculate, Calculator(), std::placeholders::_1));
    tsvr->Init();
    tsvr->Start();

    return 0;
}