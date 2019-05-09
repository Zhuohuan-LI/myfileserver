target=httpserver.exe
cc=g++ -std=c++11
CXXFLAGS=-lpthread  -I include  -g
obj=main.o epollclass.o event_handle.o eventloop.o eventloopthreadpool.o listenner.o mutex.o server.o
vpath %.h include
vpath %.cpp cpp
${target}:${obj}
	${cc} -o $@ $^ ${CXXFLAGS} 
main.o:main.cpp 
	${cc} -c ${CXXFLAGS} $^
epollclass.o:epollclass.cpp 
	${cc} -c ${CXXFLAGS} $^ 
event_handle.o:event_handle.cpp 
	${cc} -c ${CXXFLAGS} $^ 
eventloop.o:eventloop.cpp 
	${cc} -c ${CXXFLAGS} $^ 
eventloopthreadpool.o:eventloopthreadpool.cpp
	${cc} -c ${CXXFLAGS} $^ 
listenner.o:listenner.cpp
	${cc} -c ${CXXFLAGS} $^ 
mutex.o:mutex.cpp
	${cc} -c ${CXXFLAGS} $^ 
server.o:server.cpp
	${cc} -c ${CXXFLAGS} $^ 
.PHONY:clean
clean:	
	-rm ${obj}