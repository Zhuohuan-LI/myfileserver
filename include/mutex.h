#include"pthread.h"
#include<memory>
namespace myftp{

class mutex;
enum class mutextype:int
{
        Normal=PTHREAD_MUTEX_NORMAL,
        Default=PTHREAD_MUTEX_DEFAULT,
        Recursive=PTHREAD_MUTEX_RECURSIVE,
        Errorcheck=PTHREAD_MUTEX_ERRORCHECK
};


class mutexattr
{   
friend class mutex;
public:

        mutexattr();
        mutexattr& setshared();
        
        mutexattr& settype(mutextype type);
private:
        pthread_mutexattr_t mutexlockattr;
};


class mutex
{
friend class condition;
public:
        mutex();
        
        mutex(mutexattr temp);       
        int lock();   
        int trylock();
        
        int unlock(); 
        ~mutex();
       
private:
        pthread_mutex_t mutexlock;
        
};


class condition
{
public:
        condition();
        condition& wait();
        void lock();
        
        void unlock();
        
        void notify();
private:
        mutex mutexcond;
        pthread_cond_t condlock;
};
}
