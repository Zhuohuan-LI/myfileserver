#include"nocopy.h"
#include"eventloopthread.h"
// #ifndef eventloopthread_H
// #define eventloopthread_H
// #endif
#include"pthread.h"
#include<map>
#include"unistd.h"
#include<utility>
#include<sys/eventfd.h>
#include"fcntl.h"
#include<deque>
#include<memory>
namespace myftp
{
using std::pair;
using std::vector;
using std::deque;
using std::shared_ptr;
using std::make_shared;


class threadpool:public nocopyable
{
public:
        threadpool(int num);
        
        void addtask(int soc);
        
        void stop();
        void start();
        
         ~threadpool();
        
private:
        int roundrobin;
        static void* loginit(void*arg);

        int work_threads;
        vector<shared_ptr<eventloopthread>> loopthreads;
        bool quit;
        
        
        
};

}