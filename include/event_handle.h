#include"sys/socket.h"
#include<vector>
#include"errno.h"
#include<string>
#include<cstring>
#include<dirent.h>
#include"sys/stat.h"
#include"sys/time.h"
#include<cassert>
#include<iostream>
#include"unistd.h"
#include"fcntl.h"
namespace myftp
{
class event_handle
{
public:
        event_handle()=default;
        event_handle(int x);
        
        //若发完了，关掉EPOLLOUT
        bool sendmessage();
       
        //若向发送缓冲区填入了数据，打开EPOLLOUT
        int receivemessage();
        bool registered;
      
private:
         void urlprocessing(char* urlstr,int len);
        static unsigned char char_to_hex( unsigned char x );
        static bool is_alpha_number_char( unsigned char c );
        //url编码实现   
        static int urlencode( unsigned char * src, int  src_len, unsigned char * dest, int  dest_len );
        //解url编码实现  :输出返回字符串的长度  
        static  int urldecode(unsigned char* encd,unsigned char* decd) ;  
        static std::string html;
        static std::string registerhtml;

        int fd;//socket
        std::vector<std::string> sendtask;
        int filefd;
        std::string recvbuf;
        std::string sendbuf;
        

};

}