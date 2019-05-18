#include"eventloopthreadpool.h"
#include"listenner.h"
namespace myftp
{
class server:public nocopyable
{
public:
        server();

        server(addrinfo x,int y);
        void start();
        
private:
        addrinfo address;
        listenner listen;
        threadpool pool;
        epoll ep;
};

}