#include"eventloopthreadpool.h"
namespace myftp
{
        threadpool::threadpool(int num):max_threads(num),quit(0),index(0)
        {
                for(int i=0;i<max_threads;i++)
                {
                        int fd=eventfd(0,O_NONBLOCK);//eventfd
                       
                        loopdata x{1,fd};
                        shared_ptr<loopdata>y;
                        y=make_shared<loopdata>(x);
                        sharedlist.emplace_back(y);
                        pthread_t temp;
                        pthread_create(&temp,nullptr,init,(void*)&(sharedlist[i]));
                        
                }
        }
         threadpool::~threadpool()
        {
                stop();
        }
        void threadpool::addtask(int soc)
        {           
                sharedlist[index]->lock.lock();
                int fd=sharedlist[index]->wakefd;          
                sharedlist[index]->socklist.push_back(soc);
                sharedlist[index]->lock.unlock();
                uint64_t x=1;
                write(fd,&x,sizeof(uint64_t));
                index+=1;
                if(index>=max_threads)
                        index=0;                    
        }
        void threadpool::stop()
        {
                for(int i=0;i<max_threads;i++)
                {
                        sharedlist[i]->lock.lock();
                        sharedlist[i]->run=0;
                        sharedlist[i]->lock.unlock();
                }
        }
       
        void* threadpool::init(void*arg)
{
        shared_ptr<loopdata>* item=(shared_ptr<loopdata>*)(arg);
        eventloop thloop(*item);
        while(thloop.info->run)
        {
                thloop.loop();              
        }
}
}