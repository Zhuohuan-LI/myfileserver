#include"sys/epoll.h"
#include"unistd.h"
#include<vector>
#include<cassert>
#include<iostream>
#pragma once
namespace myftp
{
        enum class Option
        {
                Add=EPOLL_CTL_ADD,
                Mod=EPOLL_CTL_MOD,
                Del=EPOLL_CTL_DEL
        };
        
        class epoll
        {
        public:
                epoll();
                
                int add(int socfd, epoll_event eve);
                int del(int socfd,epoll_event eve);
                
                int mod(int socfd,epoll_event eve);
                
                int  wait(std::vector<epoll_event>&result);
                
                
                ~epoll();
                
        public:
                static constexpr __uint32_t IN=EPOLLIN;
                static constexpr __uint32_t OUT=EPOLLOUT;
                static constexpr __uint32_t RDHUP=EPOLLRDHUP;
                static constexpr __uint32_t PRI=EPOLLPRI;
                static constexpr __uint32_t ERR=EPOLLERR;
                static constexpr __uint32_t HUP=EPOLLHUP;
                static constexpr __uint32_t ET=EPOLLET;
                static constexpr __uint32_t OENSHOT=EPOLLONESHOT;
                
        private:           
                const int initsize=16;
                int epfd;
                
                
        };
}