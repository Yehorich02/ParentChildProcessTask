#include "Parent.h"

bool ParentProc::is_alive = false;

ParentProc::ParentProc()
{
    {
        std::lock_guard<std::mutex> cout_lock(cout_mtx);
        std::cout << "The parent process >>> started" << std::endl;
    }
    internal_counter = 0;
    is_alive = true;
    childProc_status = 0;

    if(counterFile() != 0)
    {
        throw;
    }

    interCounter_thread = std::thread(&ParentProc::internalCounter, this);
}


void ParentProc::funcForChildProc()
{
    while (is_alive)
    {
        childProc_id = fork();
        if (childProc_id == -1)
        {
            is_alive = false;
            throw std::runtime_error("The child process is not created. Errno is equal " + errno);
        }

        if (childProc_id == 0)
        {
            execChildProc();
        }
        else
        {
            waitpid(childProc_id, &childProc_status, 0);
            std::lock_guard<std::mutex> cout_lock(cout_mtx);
            std::cout << "The child process " << childProc_id << " >>> finished." << std::endl;
        }
    }
}


void ParentProc::signalHandler(int sig)
{
    ParentProc::is_alive = false;
}


int32_t ParentProc::counterFile()
{
    std::ofstream counter_file("CounterFile.txt");
    if(counter_file.is_open())
    {
        counter_file << "0";
        counter_file.close();
    }
    else
    {
        is_alive = false;
        throw std::runtime_error("Unable to open CounterFile.txt");
    }

    return 0;
}


void ParentProc::execChildProc()
{
    std::string time_period;

    {
        std::lock_guard<std::mutex> cout_lock(cout_mtx);
        std::cout << "Child proc " << getpid() << " started after forking" << std::endl;
        std::cout << "Please enter time in miliseconds ";
        std::cin >> time_period;
    }

    if (std::atoi(time_period.c_str()) < 1 || std::atoi(time_period.c_str()) > 1000)
    {
        is_alive = false;
        throw std::runtime_error("The specified time is out of range [1ms, 1s]");
    }

    std::cout << execl("child", "child", time_period.c_str(), NULL) << std::endl;
}


void ParentProc::internalCounter()
{
    while(is_alive)
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(1000));
        {
            std::lock_guard<std::mutex> intercounter_lock(interCounter_mtx);
            internal_counter++;
        }

        if (internal_counter % 10 == 0)
        {
            std::lock_guard<std::mutex> cout_lock(cout_mtx);
            std::cout << "The parent process runs for " << internal_counter << " seconds" << std::endl;
        }
    }
}


ParentProc::~ParentProc()
{
    if(interCounter_thread.joinable())
    {
        interCounter_thread.join();
    }

    std::lock_guard<std::mutex> cout_lock(cout_mtx);
    std::cout << "The parent process worked for " << internal_counter << " seconds" << std::endl;
    std::cout << "The parent process >>> finished" << std::endl;
}


int main()
{
    try
    {
        std::signal(SIGINT, ParentProc::signalHandler);
        std::unique_ptr<ParentProc> parent(new ParentProc());
        parent->funcForChildProc();
    }
    catch(const std::exception& ex)
    {
        std::cerr << ex.what() << std::endl;
    }
    return 0; 
}
