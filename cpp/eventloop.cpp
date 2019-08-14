#include"eventloop.h"
namespace myftp{
        eventloop::eventloop():ep(),lock_()
        {     
                wakefd=eventfd(0,O_NONBLOCK);//wakefd
                epoll_event eve;
                eve.data.fd=wakefd;
                eve.events=epoll::IN;
                ep.add(wakefd,eve);
        }
        //ioop->返回就绪事件->对所有就绪事件判断fd->若为wake则。。否则判断事件类型
        void eventloop:: loop()
        {
               std:: vector<epoll_event> eve;//保存内核返回的就绪事件
               eve.reserve(connections_.size()+1);
                int num=ep.wait(eve);
                
                for(int i=0;i<num;i++)
                {      
                        int activefd=eve[i].data.fd;
                        if(activefd==wakefd)
                        {       
                                uint64_t y;
                                read(wakefd,&y,sizeof(uint64_t));//
                                lock_.lock();
                                //取走所有的新分配的socket
                                int socnum=soctask.size();
                                for(int i=0;i<socnum;i++)
                                {
                                        //向epoll中注册事件
                                        int confd=soctask[i];
                                        connection x(confd);
                                        auto res=connections_.insert({confd,x});
                                        if(res.second==0)
                                                throw res;
                                        epoll_event newevent;
                                        newevent.events=epoll::IN;
                                        newevent.data.fd=confd;

                                        
                                        ep.add(confd,newevent);
                                        LOGINFO<<"new connection";

                                        // log->loginfo("new connection",__FILE__,__LINE__);
                                }
                                soctask.clear();
                                lock_.unlock();
                        }
                        else
                        {
                                auto it=connections_.find(activefd);
                                        if(eve[i].events==EPOLLIN)//收到事件：可读
                                        {
                                                int flag=it->second.receivemessage();
                                                 if(flag==1)//已向发缓冲区中填入
                                                 {      //打开可写事件EPOLLOUT
                                                         epoll_event newevent;
                                                         newevent.events=EPOLLOUT|EPOLLIN;
                                                         newevent.data.fd=activefd;
                                                         ep.mod(activefd,newevent);
                                                 }
                                                  if(flag==-1)//对方断开连接
                                                  {
                                                          it->second.sendmessage();
                                                          epoll_event newevent;
                                                          int flag=ep.del(activefd,newevent);
                                                          assert(flag==0);
                                                          close(activefd);
                                                          int num=connections_.erase(activefd);
                                                          assert(num);
                                                          LOGINFO<<"connection closed";
                                                        //   log->loginfo("connection closed",__FILE__,__LINE__);
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
                                                         newevent.data.fd=activefd;
                                                         ep.mod(activefd,newevent);
                                                }
                                         }       
                                        if(eve[i].events==(EPOLLIN|EPOLLOUT))
                                        {
                                                int flag=it->second.receivemessage();
                                                if(flag==-1)
                                                {
                                                        it->second.sendmessage();
                                                        epoll_event newevent;
                                                        int flag=ep.del(activefd,newevent);
                                                        assert(flag==0);
                                                        close(activefd);
                                                        int num=connections_.erase(activefd);
                                                        assert(num);
                                                        
                                                }
                                                else
                                                {
                                                      bool flag2= it->second.sendmessage();
                                                        if(flag2)
                                                        {
                                                                epoll_event newevent;
                                                                newevent.events=EPOLLIN;
                                                                newevent.data.fd= activefd;
                                                                ep.mod( activefd,newevent);
                                                        }  
                                                }
                                        }
                                }
                        }
                }
}