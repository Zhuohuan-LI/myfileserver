#include"event_handle.h"
namespace myftp{
event_handle::event_handle(int x):fd(x){
        sendbuf.reserve(5000);
        recvbuf.reserve(1000);
}
bool event_handle::sendmessage(){

        int size;
                                              
        while(sendbuf.size()>0&&(size=send(fd,&*sendbuf.begin(),sendbuf.size(),0))>0)
        {
                sendbuf.erase(0,size);
        }
        if(sendbuf.size()==0)
        {
                return 1;
        }
        return 0;
}
int event_handle:: receivemessage(){

        int bytes=0;
        char temp[401];
        
        
        while((bytes=read(fd,temp,400))>0)
        {
                recvbuf.append(temp,0,bytes);
        }

        if(bytes==0)
        {
               return -1; 
        }
        else 
        {
                if(errno==EWOULDBLOCK)
                {
                        std::string str1="GET";
                        std::string str2="HTTP";
                        int index1;
                        int index2;
                        
                        while( 
                                        index1=recvbuf.find(str1),
                                        index2=recvbuf.find(str2),
                                        index1!=std::string::npos&&index2!=std::string::npos
                                        )
                        {
                                std::string url;
                                url.assign(recvbuf.begin()+index1+4,recvbuf.begin()+index2-1);//取出url
                                unsigned char urlstr[url.size()*4];
                                int len=urldecode((unsigned char*)&*url.begin(),urlstr);//解码url
                                
                                struct stat urlinfo;
                                int flag=stat((char*)urlstr,&urlinfo);
                                
                                if(flag==0)//success
                                {       
                                        int urlisdir=S_ISDIR(urlinfo.st_mode);
                                        if(urlisdir)
                                        {
                                                if(urlstr[len-1]!='/')//检查目录名是否正确
                                                {
                                                        urlstr[len]='/';
                                                        urlstr[len+1]='\0';
                                                }
                                                DIR *dir=opendir((char*)urlstr);
                                                if(dir ==nullptr)
                                                        throw "opendir fail";
                                                std::string sendstr;//报文实体
                                                sendstr.append(html);
                                                struct dirent*result=NULL;
                                                struct dirent temp;
                                                while(!readdir_r(dir,&temp,&result)&&result!=NULL)
                                                {
                                                        struct stat fileinfo;
                                                        std::string filepath;//完整路径
                                                        filepath.append((char*)urlstr);
                                                        filepath.append(temp.d_name); 
                                                        
                                                        if(stat(&*filepath.begin(),&fileinfo)<0)
                                                                throw "stat fail";
                                                        off_t filesize=fileinfo.st_size;

                                                        int isdir=S_ISDIR(fileinfo.st_mode);

                                                        sendstr.append("<script>addRow(\"");
                                                        // for(int i=0;temp.d_name[i]!=0&&i<256;i++)
                                                        // sendstr.push_back(temp.d_name[i]);//文件名
                                                        sendstr.append(temp.d_name);
                                                        sendstr.append("\",\"");
                                                        unsigned char dirstr[256];
                                                        int x=urlencode((unsigned char*)temp.d_name,256,dirstr,256);
                                                        // for(int i=0;i<x;i++)
                                                        // sendstr.push_back(dirstr[i]);//文件名对应的url
                                                        sendstr.append((char*)dirstr);
                                                        sendstr.append("\",");
                                                        std::string s=std::to_string(isdir);
                                                        sendstr.append(s);//是否为目录
                                                        sendstr.append(",0,\"");      //                                     
                                                        sendstr.append(std::to_string(filesize));//文件大小
                                                        sendstr.append("\",0,\"");
                                                         struct tm timeinfo;
                                                        gmtime_r(&fileinfo.st_mtim.tv_sec,&timeinfo);
                                                        char timestr[30];
                                                        int timestrsize=strftime(timestr,30,"%c",&timeinfo);
                                                        sendstr.append(timestr);
                                                        // for(int i=0;i<timestrsize;i++)
                                                        // {
                                                        //         sendstr.push_back(timestr[i]);//时间
                                                        // }
                                                        sendstr.append("\");</script>\n");

                                                }
                                                sendbuf.append("HTTP/1.1 200 OK\nContent-Length: ");
                                                sendbuf.append(std::to_string(sendstr.size()));
                                                sendbuf.append("\nContent-Type: text/html\n\n");
                                                sendbuf.append(sendstr);
                                        }else
                                        {
                                                std::string sendstr;//报文实体
                                                int openfd=open((char*)urlstr,O_RDONLY,0);
                                                if(openfd==-1)
                                                        throw "openfile fail";
                                                char readbuf[1001];
                                                int readsize=0;
                                                while(readsize=read(openfd,readbuf,1000),readsize>0)
                                                {
                                                        // sendstr.append(readbuf,0,readsize);
                                                        sendstr.insert(sendstr.size(),readbuf,readsize);
                                                }
                                                sendbuf.append("HTTP/1.1 200 OK\nContent-Length: ");
                                                sendbuf.append(std::to_string(sendstr.size()));
                                                sendbuf.append("\nContent-Type: application/octet-stream\n\n");
                                                sendbuf.append(sendstr);
                                        }                                       
                                }else
                                {
                                        sendbuf.append("HTTP/1.1 404 Not Found\nContent-Length: 40\n\
                                        Content-Type: text/html\n\nSorry,the file not found on this server\n");
                                }
                                recvbuf.erase(0,index2+8);
                        
                        }                        
                        recvbuf.clear();
                }
                return 1;
        }            
}
unsigned char event_handle::char_to_hex( unsigned char x )   
{   
        return (unsigned char)(x > 9 ? x + 55: x + 48);   
} 
 bool event_handle::is_alpha_number_char( unsigned char c )   
{   
        if ( (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || (c >= '0' && c <= '9') )   
                return 1;   
        return 0;   
}  
int event_handle::urlencode( unsigned char * src, int  src_len, unsigned char * dest, int  dest_len )   
{   
        unsigned char ch;   
        int  len = 0;
        int  reslen=0;                 
        while (len < (dest_len - 4) && *src)   
        {   
                ch = (unsigned char)*src;   
                if (*src == ' ')   
                {   
                        *dest++ = '+';   
                        reslen++;
                }   
                else if (is_alpha_number_char(ch) || strchr("=!~*'()", ch))   
                {   
                        *dest++ = *src;   
                        reslen++;
                }   
                else   
                {   
                        *dest++ = '%';   
                        reslen++;
                        *dest++ = char_to_hex( (unsigned char)(ch >> 4) );   
                        reslen++;
                        *dest++ = char_to_hex( (unsigned char)(ch % 16) );   
                        reslen++;
                }    
                ++src;   
                ++len;   
        }   
        *dest = 0;
        return  reslen;   
}   
int event_handle::urldecode(unsigned char* encd,unsigned char* decd)   
{   
        int j,i;   
        char *cd =(char*) encd;   
        char p[2];   
        unsigned int num;   
        j=0;   

        for( i = 0; i < strlen(cd); i++ )   
        {   
                // memset( p, '/0', 2 );   
                if( cd[i] != '%' )   
                {   
                decd[j++] = cd[i];   
                continue;   
                }           
                p[0] = cd[++i];   
                p[1] = cd[++i];   

                p[0] = p[0] - 48 - ((p[0] >= 'A') ? 7 : 0) - ((p[0] >= 'a') ? 32 : 0);   
                p[1] = p[1] - 48 - ((p[1] >= 'A') ? 7 : 0) - ((p[1] >= 'a') ? 32 : 0);   
                decd[j++] = (unsigned char)(p[0] * 16 + p[1]);   
        }   
        decd[j] = 0;    
        return j;  
}  
std::string event_handle::html="\
<html i18n-values=\"dir:textdirection;lang:language\">\
<head>\
<meta charset=\"utf-8\">\
<meta name=\"google\" value=\"notranslate\">\
<script>\
function addRow(name, url, isdir,size, size_string, date_modified, date_modified_string) \
{\
        if (name == \".\")\
                return;\
        var root = document.location.pathname;\
        if (root.substr(-1) !== \"/\")\
                root += \"/\";\
        var tbody = document.getElementById(\"tbody\");\
        var row = document.createElement(\"tr\");\
        var file_cell = document.createElement(\"td\");\
        var link = document.createElement(\"a\");\
        link.className = isdir ? \"icon dir\" : \"icon file\";\
        if (0) {\
                link.href = root + \"..\";\
                link.innerText = document.getElementById(\"parentDirText\").innerText;\
                link.className = \"icon up\";\
                size = 0;\
                size_string = \"\";\
                date_modified = 0;\
                date_modified_string = \"\";\
        } \
        else \
        {\
                if (isdir) \
                {\
                        name = name + \"/\";\
                        url = url + \"/\";\
                        size = 0;\
                        size_string = \"\";\
                }\
                else\
                {\
                        link.draggable = \"true\";\
                        link.addEventListener(\"dragstart\", onDragStart, false);\
                }\
                link.innerText = name;\
                link.href = root + url;\
        }\
        file_cell.dataset.value = name;\
        file_cell.appendChild(link);\
        row.appendChild(file_cell);\
        row.appendChild(createCell(size, size_string));\
        row.appendChild(createCell(date_modified, date_modified_string));\
        tbody.appendChild(row);\
}\
function onDragStart(e) {\
        var el = e.srcElement;\
        var name = el.innerText.replace(\":\", \"\");\
        var download_url_data = \"application/octet-stream:\" + name + \":\" + el.href;\
        e.dataTransfer.setData(\"DownloadURL\", download_url_data);\
        e.dataTransfer.effectAllowed = \"copy\";\
}\
function createCell(value, text) {\
        var cell = document.createElement(\"td\");\
        cell.setAttribute(\"class\", \"detailsColumn\");\
        cell.dataset.value = value;\
        cell.innerText = text;\
        return cell;\
}\
function onListingParsingError() {\
        var box = document.getElementById(\"listingParsingErrorBox\");\
        box.innerHTML = box.innerHTML.replace(\"LOCATION\", encodeURI(document.location)+ \"?raw\");\
        box.style.display = \"block\";\
}\
</script>\
<title id=\"title\"></title>\
</head>\
<body>\
<div id=\"listingParsingErrorBox\" i18n-values=\".innerHTML:listingParsingErrorBoxText\"></div>\
<span id=\"parentDirText\" style=\"display:none\" i18n-content=\"parentDirText\"></span>\
<h1 id=\"header\" i18n-content=\"header\"></h1>\
<table>\
  <thead>\
    <tr class=\"header\" id=\"theader\">\
      <th i18n-content=\"headerName\" onclick=\"javascript:sortTable(0);\"></th>\
      <th class=\"detailsColumn\" i18n-content=\"headerSize\" onclick=\"javascript:sortTable(1);\"></th>\
      <th class=\"detailsColumn\" i18n-content=\"headerDateModified\" onclick=\"javascript:sortTable(2);\"></th>\
    </tr>\
  </thead>\
  <tbody id=\"tbody\">\
  </tbody>\
</table>\
</body>\
</html>";
}