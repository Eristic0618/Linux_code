#include "Httpserver.hpp"
#include <memory>

int main(int argc, char* argv[])
{
    if(argc != 2)
    {
        exit(1);
    }

    uint16_t serverport = std::stoi(argv[1]);

    std::unique_ptr<HttpServer> psvr(new HttpServer(serverport));
    psvr->Start();
}
