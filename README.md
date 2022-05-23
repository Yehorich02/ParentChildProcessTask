# ParentChildProcessTask
## How to run the program 
0) Create the **build** folder via **mkdir build** command.
1) Go to a folder **build**
2) Build a project **cmake --build .**
3) Run the program with **./parent**
## How to interact with the program?
After the "Child" process is started, you will need to specify the timer required for the internal counter.  

The "Parent" and "Child" process can be stopped, end the program, by pressing **Ctrl+C**.

The "Child" process can be killed from the terminal with the ***kill -SIGTERM pid*** command or in case of any error.
