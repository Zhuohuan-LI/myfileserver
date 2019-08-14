#include"eventloop.h"
#include"functional"
using std::function;
namespace myftp
{
        class eventloopthread
        {
                friend class threadpool;
                public:
                        eventloopthread();
                        void start();
                        void addcon(int fd);
                        void stop();
                private:
                        static void *threadfunc(void *);
                        void loopstart();
                        eventloop *loop_;
                        mutex lock;
                        condition cond;
                        pthread_t thread;
                        bool run;
                        function<void()> runinthreadfunc;

        };
}