#include"server.h"
#include"syslog.h"
#include"stdlib.h"
#include"sys/signal.h"
int main(int argc,char *argv[])
{
        
        // pid_t pid;
        // if(pid=fork(),pid<0)
        //         return(-1);
        // else if(pid>0)
        //         _exit(0);
        // if(setsid()<0)
        //         return -1;
        // struct sigaction act;
        // struct sigaction oact;
        // act.sa_handler=SIG_IGN;
        // sigaction(SIGHUP,&act,&oact);
        // if(pid=fork(),pid<0)
        //         return -1;
        // else if(pid>0)
        //         _exit(0);
        // chdir("/");
        // for(int i=0;i<64;i++)
        //         close(i);
        // open("/dev/null",O_RDONLY);
        // open("/dev/null",O_RDWR);
        // open("/dev/null",O_RDWR);
        // openlog(argv[0],LOG_PID,0);

        myftp::log_consumer logger(myftp::Logtype::INFO);
        logger.start();

        myftp::addrinfo add(myftp::Domain::INET,"127.0.0.1");
        myftp::server ftpserve(add,1);
        ftpserve.start();
        return 0;
}
