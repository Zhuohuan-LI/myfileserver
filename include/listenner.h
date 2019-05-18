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

class addrinfo
{
friend class listenner;
public:
        addrinfo(Domain x, std::string str);

        sockaddr *address;
private:       
        Domain domain;
        sockaddr_in inetadd;
        sockaddr_in6 inet6add;
};
class listenner
{
public:
        listenner(addrinfo x);
               
        void listen();
        
        int accept();
       
        int &socket();
        
private:
        int sockfd;
        Domain domain;
        
};
}