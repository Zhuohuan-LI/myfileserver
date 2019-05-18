#include<string>
#include<vector>
#include"fcntl.h"
#include"unistd.h"
#include<memory>

#include"epollclass.h"
#include"mutex.h"
namespace myftp
{
    using std::vector;
    using std::string;
    using std::shared_ptr;
    enum class Logtype
    {
        INFO,
        DEBUG
    };
    class log_producer
    {
        friend class log_consumer;
        public:
                log_producer(int fd);
                void loginfo(string str,string filestr,int line);
                
                void logdebug(string str,string filestr,int line);
                
        private:
                void processing(string str,Logtype type);
               
        private:
            int logfd;
            mutex loglock;
            std::vector<std::string> logbuf;
            Logtype loggingtype=Logtype::INFO;
            
    };
    class log_consumer
    {
        public:
            log_consumer();
            void loginit(log_producer* x);
            
            void logloop();
        private:
            epoll ep;
            string to_file;
            int fdnum=0;
            int filefd;

    };
}