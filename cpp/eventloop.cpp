#include"eventloop.h"
namespace myftp{
        eventloop::eventloop(shared_ptr<loopdata>x):ep(),info(x)
        {     
                int fd=info->wakefd;

                epoll_event eve;
                eve.data.fd=fd;
                eve.events=epoll::IN;
                ep.add(fd,eve);
        }
        //ioop->返回就绪事件->对所有就绪事件判断fd->若为wake则。。否则判断事件类型
        void eventloop:: loop()
        {
               std:: vector<epoll_event> eve;//保存内核返回的就绪事件
               eve.reserve(socevent.size()+1);
                int num=ep.wait(eve);
                
                for(int i=0;i<num;i++)
                {      
                        int fd=eve[i].data.fd;
                        if(fd==info->wakefd)
                        {       
                                uint64_t y;
                                read(fd,&y,sizeof(uint64_t));//
                                info->lock.lock();
                                //取走所有的新分配的socket
                                for(int i=0;i<(*info).socklist.size();i++)
                                {
                                        //向epoll中注册事件
                                        int confd=(*info).socklist[i];
                                        epoll_event newevent;
                                        newevent.events=epoll::IN;
                                        newevent.data.fd=confd;

 

                                        //构建socket对应的处理器，并保存起来，留待调用
                                        event_handle handle(confd);
                                        std::pair<int ,event_handle> temp{confd,handle};
                                        auto item=socevent.emplace(temp);
                                        if(item.second==0)
                                                throw item;
                                        ep.add(confd,newevent);
                                        // assert(0);
                                }
                                info->socklist.clear();
                                info->lock.unlock();
                        }
                        else
                        {
                                auto it=socevent.find(fd);
                                if(it!=socevent.end())
                                {
                                        if(eve[i].events==EPOLLIN)//收到事件：可读
                                        {
                                                 int flag=it->second.receivemessage();//读消息并处理
                                                 if(flag==1)//已向发缓冲区中填入
                                                 {      //打开可写事件EPOLLOUT
                                                         epoll_event newevent;
                                                         newevent.events=EPOLLOUT|EPOLLIN;
                                                         newevent.data.fd=fd;
                                                         ep.mod(fd,newevent);
                                                 }
                                                  if(flag==-1)//对方断开连接
                                                  {
                                                          epoll_event newevent;
                                                          int flag=ep.del(fd,newevent);
                                                          assert(flag==0);
                                                          int num=socevent.erase(fd);
                                                          assert(num);
                                                  }
                                                //  assert(0);
                                        }
                                               
                                        if(eve[i].events==EPOLLOUT)//收到事件：可写
                                         {
                                                bool flag=it->second.sendmessage();
                                                if(flag)//判断缓冲区是否为空
                                                {
                                                        epoll_event newevent;
                                                         newevent.events=EPOLLIN;
                                                         newevent.data.fd=fd;
                                                         ep.mod(fd,newevent);
                                                }
                                         }       
                                        if(eve[i].events==(EPOLLIN|EPOLLOUT))
                                        {
                                                it->second.receivemessage();
                                                bool flag=it->second.sendmessage();
                                                if(flag)
                                                {
                                                        epoll_event newevent;
                                                         newevent.events=EPOLLIN;
                                                         newevent.data.fd=fd;
                                                         ep.mod(fd,newevent);
                                                }
                                        }
                                }
                        }
                        
                }
                
        }
}