#include"listenner.h"
namespace myftp{
        addrinfo::addrinfo(Domain x, std::string str):domain(x)
        {
                switch (domain)
                {
                case Domain::INET :
                         inet_pton(static_cast<int>(domain),&*str.begin(),static_cast<void *>(&inetadd.sin_addr)); 
                         inetadd.sin_port=htons(80);   
                         inetadd.sin_family=static_cast<int>(domain);
                         address=(sockaddr*)&inetadd;
                        break;
                case Domain::INET6:
                        inet_pton(static_cast<int>(domain),&*str.begin(),static_cast<void *>(&inet6add.sin6_addr)); 
                         inet6add.sin6_port=htons(80);   
                         inet6add.sin6_family=static_cast<int>(domain);
                         address=(sockaddr*)&inet6add;
                
                default:
                        break;
                }

        }

        listenner::listenner(addrinfo address):domain(address.domain)
        {
                sockfd=::socket(static_cast<int>(domain),SOCK_STREAM,0);
                int reuse=1;
                setsockopt(sockfd,SOL_SOCKET,SO_REUSEADDR,&reuse,sizeof(int));
                sockaddr  *add=address.address;
                bind(sockfd,add,sizeof(sockaddr));
        }
        
        void listenner::listen()
        {
                ::listen(sockfd,SOMAXCONN);
        }
        int listenner::accept()
        {
                int confd=::accept(sockfd,nullptr,nullptr);
                // int opt=fcntl(confd,F_GETFL );
                // opt |= O_NONBLOCK;
                // fcntl(sockfd, F_SETFL, opt);
                fcntl(confd, F_SETFL, O_NONBLOCK);
                return confd;
        }
        int &listenner::socket()
        {
                return sockfd;
        }
}