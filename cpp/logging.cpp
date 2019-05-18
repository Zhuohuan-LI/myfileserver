#include"logging.h"
namespace myftp
{
    log_producer::log_producer(int fd):logfd(fd)
    {

    }
    void log_producer:: loginfo(string str,string filestr,int line)
    {
        str.append(", INFO");
        str.append(", ").append(filestr);
        str.append(", ").append(std::to_string(line));
        processing(str,Logtype::INFO);
    }
    void log_producer::logdebug(string str,string filestr,int line)
    {
        str.append(", DEBUG");
        str.append(", ").append(filestr);
        str.append(", ").append(std::to_string(line));
        processing(str,Logtype::DEBUG);
    }
    void log_producer::processing(string str,Logtype type)
    {
            if(type==loggingtype)
            {
                    time_t timeptr;
                    if(time(&timeptr)==-1)
                            throw "time fail";
                    struct tm gmptr;
                    localtime_r(&timeptr,&gmptr);
                    char timebuf[36];
                    strftime(timebuf,35,"%c",&gmptr);
                    str.append(timebuf).append("\r\n");


                    loglock.lock();
                    logbuf.push_back(str);
                    loglock.unlock();
                    uint64_t x=1;
                    write(logfd,&x,sizeof(uint64_t));

            }                        
    }
    log_consumer::log_consumer()
    {
        filefd=open("./logfile",O_RDWR|O_APPEND|O_CREAT,666);
        if(filefd==-1)
            throw "fileopen fail";
        
    }
    void  log_consumer::loginit(log_producer* x)
    {
        epoll_event eve;
        eve.events=epoll::IN;
        eve.data.ptr=x;
        ep.add(x->logfd,eve);
        fdnum=fdnum+1;
    }
    void log_consumer::logloop()
    {
        std:: vector<epoll_event> eve;//保存内核返回的就绪事件
        eve.reserve(fdnum);
        int num=ep.wait(eve);
        for(int i=0;i<num;i++)
        {
            if(eve[i].events==epoll::IN)
            {
                log_producer *x=(log_producer *)eve[i].data.ptr;

                uint64_t y;
                read(x->logfd,&y,sizeof(uint64_t));//

                x->loglock.lock();
                vector<string> &xx=x->logbuf;
                for(int j=0;j<xx.size();j++)
                    to_file.append(xx[j]);
                write(filefd,&*to_file.begin(),to_file.size());
                to_file.clear();
            }
        }
    }
}