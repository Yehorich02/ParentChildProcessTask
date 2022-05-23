#include "Child.h"

ChildProc::ChildProc(uint32_t time): time_period(time)
{   
    is_alive = true;
    if(counterFileRead() != 0)
    {
        throw;
    }
    std::lock_guard<std::mutex> cout_lock(cout_mtx);
    std::cout << "The child process " << getpid() << " >>> started" << std::endl;
}


void ChildProc::interCounterOutput()
{
    while (is_alive)
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(1000));
        std::lock<std::mutex>(cout_mtx, interCounter_mtx);

        std::lock_guard<std::mutex> interCounter_lock(interCounter_mtx, std::adopt_lock);
        std::lock_guard<std::mutex> cout_lock(cout_mtx, std::adopt_lock);

        std::cout << "Internal counter in the child process " << getpid() <<  " equal " << internal_counter << std::endl;
    }
}


int32_t ChildProc::counterFileRead()
{
    std::ifstream counter_file("CounterFile.txt");
    if(counter_file.is_open())
    {
        std::string line;
        std::getline(counter_file, line);
        internal_counter = std::atoi(line.c_str());
        counter_file.close();
    } 
    else
    {
        throw std::runtime_error("Unable to open CounterFile.txt");
    }

    return 0;
}


void ChildProc::internalCounter()
{
    interCounter_thread = std::thread(&ChildProc::interCounterOutput, this);
    
        while (is_alive)
        {
            std::this_thread::sleep_for(std::chrono::milliseconds(time_period));
            std::lock_guard<std::mutex> interCounter_lock(interCounter_mtx);
            internal_counter++;

            std::ofstream counter_file("CounterFile.txt");

            if (counter_file.is_open())
            {
                counter_file.clear();
                counter_file << internal_counter;
                counter_file.close();
            }
            else
            {
                is_alive = false;
                throw std::runtime_error("Unable to open CounterFile.txt for recording");
            }
        }       
}


ChildProc::~ChildProc()
{
    if (interCounter_thread.joinable())
    {
        interCounter_thread.join();
    }
}

int main(int argc, char *argv[])
{
    try
    {
        std::unique_ptr<ChildProc> child(new ChildProc(std::atoi(argv[1])));
        child->internalCounter();
    }
    catch(std::exception& ex)
    {
        std::cerr << ex.what() << std::endl;

    }

    return 0;
}