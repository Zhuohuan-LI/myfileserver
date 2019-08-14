#include<string>
#include<vector>
#include"fcntl.h"
#include"unistd.h"
#include<memory>
#include"sys/eventfd.h"
#include<functional>

#include"epollclass.h"
#include"mutex.h"
namespace myftp
{
    using std::vector;
    using std::string;
    using std::shared_ptr;
    using std::function;
    enum class Logtype
    {
        INFO,
        DEBUG
    };
    class log_consumer
    {
        public:
            log_consumer(Logtype);
            void start();
            void delivery(std::string);

            void logloop();
             Logtype loggingtype;
        private:
            static void*threadfunc(void*);
            pthread_t thread_;
            string to_file;
            int filefd;
            mutex lock;
            condition cond;
            function<void()>runinthreadfunc;
            bool run;
    };
    class log_producer
    {
        friend class log_consumer;
        public:
                log_producer(Logtype,int ,string);
                void logging(string str);
                void operator<<(std::string);
                ~log_producer();
        private:
                void processing(string str);
               
        private:
            static log_consumer* consumerptr;
            std::string logbuf;
            Logtype loggingtype;
            int linenum;
            std::string filestr;
            
    };
    

}
#define LOGINFO myftp::log_producer(myftp::Logtype::INFO,__LINE__,__FILE__)
#define LOGDEBUG myftp::log_producer(myftp::Logtype::DEBUG,__LINE__,__FILE__)