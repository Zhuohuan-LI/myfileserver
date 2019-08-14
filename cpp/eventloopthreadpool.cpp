#include"eventloopthreadpool.h"
namespace myftp
{
        threadpool::threadpool(int num):work_threads(num),quit(0),roundrobin(0)
        {

        }
         threadpool::~threadpool()
        {
                stop();
        }
        void threadpool::start()
        {
                for(int i=0;i<work_threads;i++)
                {
                        eventloopthread* loopthread=new eventloopthread();
                        
                        loopthreads.push_back(shared_ptr<eventloopthread>(loopthread));
                        loopthreads[i]->start();
                }
               

        }
        void threadpool::addtask(int soc)
        {           
                if(roundrobin>=work_threads)
                        roundrobin=0;  
                loopthreads[roundrobin]->addcon(soc);
                roundrobin+=1;
                
                                 
        }
        void threadpool::stop()
        {
                for(int i=0;i<work_threads;i++)
                {
                        loopthreads[i]->stop();
                }
        }
        
}