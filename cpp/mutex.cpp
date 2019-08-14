#include"mutex.h"
namespace myftp{
        mutexattr::mutexattr()
        {
                pthread_mutexattr_init(&mutexlockattr);
        }
        mutexattr& mutexattr::setshared()
        {
                #ifdef _POSIX_THREAD_PROCESS_SHARED
                pthread_mutexattr_setpshared(&mutexlockattr,1);
                #endif
                return *this;
        }
        mutexattr& mutexattr::settype(mutextype type)
        {
                int x=static_cast<int> (type);
                pthread_mutexattr_settype(&mutexlockattr,x);
                return *this;
        }


        mutex::mutex()
        {
                pthread_mutex_init(&mutexlock,nullptr);
        }
        mutex::mutex(mutexattr temp)
        {
                pthread_mutex_init(&mutexlock,&temp.mutexlockattr);
        }
        mutex::~mutex()
        {
                pthread_mutex_unlock(&mutexlock);
        }
        int mutex::lock()
        {
                int res=pthread_mutex_lock(&mutexlock);
                return res;
        }
         int mutex::trylock()
        {
                int res=pthread_mutex_trylock(&mutexlock); 
                return res;
        }
        int mutex::unlock()
        {
                int res=pthread_mutex_unlock(&mutexlock);
                return res;
        }


        condition::condition():mutexcond(),condlock(PTHREAD_COND_INITIALIZER)
        {

        }
        void condition::wait()
        {
                pthread_cond_wait(&condlock,&mutexcond.mutexlock);
        }
        void condition::lock()
        {
                mutexcond.lock();
        }
        void condition::unlock()
        {
                mutexcond.unlock();
        }
        void condition::notify()
        {
                pthread_cond_signal(&condlock);
        }
}