#include"epollclass.h"
namespace myftp
{
                epoll::epoll()
                {
                        epfd=epoll_create(initsize);                      
                }
                epoll::~epoll()
                {
                        close(epfd);
                }
                int epoll::add(int socfd, epoll_event eve)
                {                       
                        return epoll_ctl(epfd,static_cast<int>(Option::Add),socfd,&eve);                       
                }
                int epoll::del(int socfd,epoll_event eve)
                { 
                       return epoll_ctl(epfd,static_cast<int>(Option::Del),socfd,&eve);
                }
                int  epoll::mod(int socfd,epoll_event eve)
                {
                        return epoll_ctl(epfd,static_cast<int>(Option::Mod),socfd,&eve);
                }
                int epoll:: wait(std::vector<epoll_event>&result)
                {       
                        int x=epoll_wait(epfd,&*result.begin(),result.capacity(),0);   
                        return x;                     
                }
}