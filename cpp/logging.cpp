#include"logging.h"
namespace myftp
{
    log_consumer* log_producer::consumerptr=nullptr;
    log_producer::log_producer(Logtype x,int y,string z):loggingtype(x),linenum(y),filestr(z)
    {
        
    }
    void log_producer::operator<<(std::string str)
    {
        logging(str);
    }
    void log_producer:: logging(string str)
    {
        if(loggingtype==Logtype::DEBUG)
            str.append(", DEBUG");
        else
            str.append(", INFO");
        
        str.append(", ").append(filestr);
        str.append(", ").append(std::to_string(linenum)).append(", ");
        processing(str);
    }
    
    void log_producer::processing(string str)
    {         
                    time_t timeptr;
                    if(time(&timeptr)==-1)
                            throw "time fail";
                    struct tm gmptr;
                    localtime_r(&timeptr,&gmptr);
                    char timebuf[36];
                    strftime(timebuf,35,"%c",&gmptr);
                    str.append(timebuf).append("\r\n");
                    logbuf.append(str);
                        
    }
    log_producer::~log_producer()
    {
        if(loggingtype==consumerptr->loggingtype)
        consumerptr->delivery(logbuf);
    }

    log_consumer::log_consumer(Logtype x):loggingtype(x)
    {
        filefd=open("./logfile",O_RDWR|O_APPEND|O_CREAT,666);
        if(filefd==-1)
            throw "fileopen fail";
        
    }
    void log_consumer::delivery(std::string str)
    {
        cond.lock();
        to_file.append(str);
        cond.unlock();
        cond.notify();
    }
    void log_consumer::start()
    {
        int flag;
        run=1;
        runinthreadfunc=std::bind(&myftp::log_consumer::logloop,this);
        if(flag=pthread_create(&thread_,nullptr,&threadfunc,&(runinthreadfunc)),flag==-1)
                                throw "pthread_create fail";
        log_producer::consumerptr=this;
        
    }
    void* log_consumer::threadfunc(void*data)
    {
        function<void()>temp=*(function<void()>*)data;
        temp();
    }
    void log_consumer::logloop()
    {
        while(run)
        {
            cond.lock();
            while (to_file.size()<10)
            {
                cond.wait();
            }
            write(filefd,&*to_file.begin(),to_file.size());
            to_file.clear();
            
        }
    }
        
}