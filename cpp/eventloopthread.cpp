#include"eventloopthread.h"
namespace myftp
{

        eventloopthread::eventloopthread():loop_(nullptr)
        {

        }
        void eventloopthread::start()
        {
                run=1;
                int flag;
                
                runinthreadfunc=std::bind(&myftp::eventloopthread::loopstart,this);
                if(flag=pthread_create(&thread,nullptr,&threadfunc,&(runinthreadfunc)),flag==-1)
                                throw "pthread_create fail";
                cond.lock();
                while(loop_==nullptr)
                        cond.wait();
        }
        void eventloopthread::addcon(int fd)
        {
                if(loop_==nullptr)
                        throw nullptr;
                loop_->lock_.lock();
                loop_->soctask.push_back(fd);
                loop_->lock_.unlock();
                uint64_t x=1;
                write(loop_->wakefd,&x,sizeof(uint64_t));
                

        }
        void eventloopthread::stop()
        {
                run=0;
        }
        void* eventloopthread::threadfunc(void * x)
        {
                function<void()>y=*(function<void()>*)x;
                y();
        }
        void eventloopthread::loopstart()
        {
                eventloop loop;
                
                cond.lock();
                loop_=&loop;
                cond.unlock();
                cond.notify();
                while (run)
                {
                        loop.loop();
                }
                
        }

}