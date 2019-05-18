// #include"epollclass.h"
// #include"mutex.h"
#include"event_handle.h"
#include"logging.h"

#include<deque>
#include"map"
#include<memory>


// #ifndef MUTEX_H
// #define MUTEX_H
// #endif
namespace myftp
{
using std::map;
using std::deque;
using std::shared_ptr;
using std::pair;
using std::make_shared;


struct loopdata
{
        bool run;
        int wakefd;
        log_producer logger;
        mutex worklock;//
        deque<int>socklist;
        
        
};
class eventloop
{
friend class threadpool;
public:
        eventloop(shared_ptr<loopdata>x);
        
        void loop();
        

private:
        epoll ep;
        shared_ptr<loopdata> info;
        map<int,event_handle> socevent;//保存线程持有的socket所对应的处理器
};
}