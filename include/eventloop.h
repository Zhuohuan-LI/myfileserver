// #include"epollclass.h"
// #include"mutex.h"
#include"connection.h"
#include"logging.h"

#include<deque>
#include"map"
#include<memory>
#include"sys/eventfd.h"

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


class eventloop
{
friend class eventloopthread;
public:
        eventloop();
        
        void loop();
        

private:
        epoll ep;
        mutex lock_;
        int wakefd;
        map<int,connection> connections_;//保存线程持有的socket所对应的处理器
        deque<int>soctask;
        
};
}