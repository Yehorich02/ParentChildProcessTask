#include "Parent.h"

ParentProc::ParentProc()
{
    {
        // !!!
    }
    internal_counter = 0;
    is_alive = true;
    childProc_status = 0;
    
    childProc_id = fork();

    if(childProc_id == -1)
    {
        std::lock_guard<std::mutex> cout_lock(cout_mtx);
        std::cout << "The child process is not created. Errno is equal " << errno << std::endl;
        throw;
    }

    interCounter_thread = std::thread(&ParentProc::internalCounter, this);

    if(childProc_id == 0)
    {
        execChildProc();        
    }
    else
    {
        std::cout << "Parent proc >>> started" << std::endl;

        while(true)
        {
            std::cout << "::)" << std::endl;
            waitpid(childProc_id, &childProc_status, 0); 

            // make review for errors

            std::cout << "End of the proc  " << childProc_id << std::endl;
            execChildProc();
            childProc_status = 0;
        }     
    }

}


void ParentProc::execChildProc()
{
    std::string time_period;
    std::cout << "Child proc >>> started after forking" << std::endl;
    std::cout << "Please enter time in miliseconds ";

    std::cin >> time_period;
    if (std::atoi(time_period.c_str()) < 1 || std::atoi(time_period.c_str()) > 1000)
    {
        throw; /// !!
    }
    std::cout << execl("main", "main", time_period.c_str(), NULL) << std::endl;
}
void ParentProc::internalCounter()
{
    while(is_alive)
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(1'000));
        {
            std::lock_guard<std::mutex> intercounter_lock(interCounter_mtx);
            internal_counter++;
        }
        
    }
}

ParentProc::~ParentProc()
{
    if(interCounter_thread.joinable())
    {
        interCounter_thread.join();
    }
    
}

int main()
{
    std::unique_ptr<ParentProc> parent(new ParentProc());
}


//int main()
//{
//    std::cout << "Parent proc >>> started" << std::endl;
//
//    pid_t pid =0;
//
//    pid = fork();
//    std::string time;
//    int status;
//    std::cout << pid << std::endl;
//
//    if(pid == 0)
//    {
//        std::cout << "Child proc >>> started after forking" << std::endl;
//        std::cout << "Please enter time in miliseconds ";
//        std::cin >> time; 
//        if(std::atoi(time.c_str()) < 1 || std::atoi(time.c_str()) > 1000) return 0;
//        std::cout << execl("main", "main" , time.c_str(), NULL) << std::endl;;
//        std::cout << "Child proc >>> finished after forking" << std::endl;
//    }
//    else{
//        std::cout << "Parent" << std::endl;
//        pid = wait(&status);
//        std::cout << "End of the proc  "<< pid << std::endl;
//    }
//    while(true)
//    {
//        
//    }
//    //switch (pid)
//    //{
//    //case -1:
//    //    perror("Trouble with fork()");
//    //    return -1;
//    //case 0:
//    //    std::cout << "Child proc >>> started after forking" << std::endl;
//    //    execl("main", "main" , "10", NULL);
//    //    std::cout << "Child proc >>> finished after forking" << std::endl;
//    //    break;
////
//    //default : break;
//    //}
//
//    return 0;
//}