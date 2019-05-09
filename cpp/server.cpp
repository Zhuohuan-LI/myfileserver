#include"server.h"
namespace myftp
{
        server::server():address(Domain::INET,std::string("127.0.0.1")),listen(Domain::INET,Type::STREAM),pool(1)
        {

        }
        void server:: start()
        {
                listen.socbind(address);
                listen.listen();
                epoll_event eve;
                eve.events=EPOLLIN;
                ep.add(listen.socket(),eve);     
                for(;;)
                {  
                        std::vector<epoll_event> accepteve;
                        accepteve.reserve(1);
                        int resnum=ep.wait(accepteve);
                        for(int i=0;i<resnum;i++)
                        {      
                                if(accepteve[i].events==EPOLLIN)
                                {
                                        int confd=listen.accept();
                                        pool.addtask(confd);
                                        // assert(0);
                                }
                                if(accepteve[i].events==EPOLLOUT)
                                {

                                }
                                if(accepteve[i].events==EPOLLERR)
                                {

                                }
                        }
                }
        }
}