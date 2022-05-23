#pragma once
#include <chrono>
#include <csignal>
#include <fstream>
#include <iostream>
#include <mutex>
#include <memory>
#include <string>
#include <thread>
#include <unistd.h>


class ChildProc
{
public:
    ChildProc(uint32_t counter);
    ~ChildProc();

    void internalCounter();

private:
    uint32_t internal_counter;
    uint32_t time_period;
    bool is_alive;

    std::mutex cout_mtx;
    std::mutex interCounter_mtx;
    
    std::thread interCounter_thread;

    int32_t counterFileRead();
    void interCounterOutput();  
};
