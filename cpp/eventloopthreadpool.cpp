#include"eventloopthreadpool.h"
namespace myftp
{
        threadpool::threadpool(int num):work_threads(num),quit(0),index(0)
        {
                int flag;
                pthread_t temp;
                for(int i=0;i<num;i++)
                {
                        int fd=eventfd(0,O_NONBLOCK);//wakefd
                        if(fd==-1)
                                throw "wakefd";
                        int fd2=eventfd(0,O_NONBLOCK);//logfd
                        if(fd2==-1)
                                throw "logfd";
                        
                        log_producer y(fd2);
                        loopdata z{1,fd,y};
                        shared_ptr<loopdata>v;
                        v=make_shared<loopdata>(z);
                        sharedlist.push_back(v);
                }
                // pthread_t temp;
                
                for(int i=0;i<num;i++)
                {
                        if(flag=pthread_create(&temp,nullptr,workinit,&(sharedlist[i])),flag==-1)
                                throw "pthread_create fail";
                }
                if(flag=pthread_create(&temp,nullptr,loginit,(void*)&(sharedlist)),flag==-1)
                        throw "pthread_create fail";

        }
         threadpool::~threadpool()
        {
                stop();
        }
        void threadpool::addtask(int soc)
        {           
                sharedlist[index]->worklock.lock();
                int fd=sharedlist[index]->wakefd;          
                sharedlist[index]->socklist.push_back(soc);
                sharedlist[index]->worklock.unlock();
                uint64_t x=1;
                write(fd,&x,sizeof(uint64_t));
                index+=1;
                if(index>=work_threads)
                        index=0;                    
        }
        void threadpool::stop()
        {
                for(int i=0;i<work_threads;i++)
                {
                        sharedlist[i]->worklock.lock();
                        sharedlist[i]->run=0;
                        sharedlist[i]->worklock.unlock();
                }
        }
       
        void* threadpool::workinit(void*arg)
        {
                shared_ptr<loopdata>* item=(shared_ptr<loopdata>*)arg;
                shared_ptr<loopdata> x(*item);
                
                eventloop thloop(*item);
                while(thloop.info->run)
                {
                        thloop.loop();              
                }
                
                
        }
        void* threadpool::loginit(void*arg)
        {
                vector<shared_ptr<loopdata>>*item=(vector<shared_ptr<loopdata>>*)(arg);
                log_consumer log;

                for(int i=0;i<item->size();i++)
                {            
                        log.loginit(&(*(*item)[i]).logger);      
                }
                while(1)
                {
                        log.logloop();
                }
        }
}