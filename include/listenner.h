#include"sys/socket.h"
#include"arpa/inet.h"
#include"netdb.h"
#include"fcntl.h"
#include<string>

namespace myftp
{
enum class Domain
{
        INET=AF_INET,
        INET6=AF_INET6,
        UNIX=AF_UNIX,
        UNSPEC=AF_UNSPEC
};
enum class Type
{
        DGRAM=SOCK_DGRAM,
        STREAM=SOCK_STREAM
};
class addrinfo
{
friend class listenner;
public:
        addrinfo(Domain domain, std::string str);

        sockaddr *address;
private:       
        sockaddr_in inetadd;
        sockaddr_in6 inte6add;
};
class listenner
{
public:
        listenner(Domain x,Type y);
        
        void socbind( addrinfo address);
        
        void listen();
        
        int accept();
       
        int &socket();
        
private:
        int sockfd;
        Domain domain;
        Type type;  
};
}