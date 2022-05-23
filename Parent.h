#pragma once
#include <chrono>
#include <csignal>
#include <fstream>
#include <iostream>
#include <mutex>
#include <memory>
#include <sys/types.h>
#include <sys/wait.h>
#include <thread>
#include <unistd.h>


class ParentProc
{
public:
    ParentProc();
    ~ParentProc();
    void funcForChildProc();

    static void signalHandler(int32_t sig);
    static bool is_alive;

private:
    pid_t childProc_id;
    uint32_t internal_counter;
    int32_t childProc_status;

    std::mutex cout_mtx;
    std::mutex interCounter_mtx;

    std::thread interCounter_thread;

    int32_t counterFile();
    void execChildProc();
    void internalCounter();
};