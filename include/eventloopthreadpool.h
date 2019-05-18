#include"nocopy.h"
#include"eventloop.h"

// #ifndef MUTEX_H
// #define MUTEX_H
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
        
         ~threadpool();
        
private:
        static void* workinit(void*arg);
        static void* loginit(void*arg);

        int work_threads;
        vector<shared_ptr<loopdata>> sharedlist;
        bool quit;
        
        int index=0; 
        
};

}