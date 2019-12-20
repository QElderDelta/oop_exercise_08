#include <iostream>

#include "pubsub.hpp"
#include "figure.hpp"
#include "rhombus.hpp"
#include "hexagon.hpp"
#include "pentagon.hpp"

std::mutex queueMutex;
std::mutex readMutex;
std::condition_variable var;
bool done = false;
unsigned bufferSize;

class ThreadFunc {
public:
    ThreadFunc(const TaskChanel& taskChanel) : taskChanel(taskChanel){};

    void addTask(const Task& task) {
        std::lock_guard<std::mutex> lock(queueMutex);
        tasks.push(task);
    }

    void operator()() {
        while(true) {
            std::unique_lock<std::mutex> mainLock(readMutex);
            if(!tasks.empty()) {
                {
                    std::lock_guard<std::mutex> lock(queueMutex);
                    Task currentTask = tasks.front();
                    tasks.pop();
                    if(currentTask.getType() == TaskType::exit) {
                        break;
                    } else {
                        taskChanel.notify(currentTask);
                    }
                    done = true;
                    var.notify_one();
                }
            }

        }
    }
private:
    std::queue<Task> tasks;
    TaskChanel taskChanel;
};

int main(int argc, char** argv) {
    if(argc != 2) {
        std::cout << "Did this so argc wouldn't be highlited red/check your input" << std::endl;
        return -1;
    }
    bufferSize = std::atoi(argv[1]);
    std::vector<std::shared_ptr<Figure>> figures;
    std::string command;
    int command2;

    std::shared_ptr<Subscriber> consolePrint(new ConsolePrinter());
    std::shared_ptr<Subscriber> filePrint(new FilePrinter());
    
    TaskChanel taskChanel;
    taskChanel.subscribe(consolePrint);
    taskChanel.subscribe(filePrint);
    
    ThreadFunc func(taskChanel);
    std::thread thread(std::ref(func));

    while(std::cin >> command) {
        if(command == "exit") {
            func.addTask({TaskType::exit, figures});
            break;
        } else if(command == "add") {
            std::shared_ptr<Figure> f;
            std::cout << "1 - Rhombus, 2 - Pentagon, 3 - Hexagon" << std::endl;
            std::cin >> command2;
            try {
                if(command2 == 1) {
                    f = std::make_shared<Rhombus>(Rhombus(std::cin));
                } else if(command2 == 2) {
                    f = std::make_shared<Pentagon>(Pentagon{std::cin});
                } else if(command2 == 3) {
                    f = std::make_shared<Hexagon>(Hexagon{std::cin});
                } else {
                    std::cout << "Wrong input" << std::endl;
                }
                figures.push_back(f);
            } catch(std::exception& e) {
                std::cerr << e.what() << std::endl;
            }
            if(figures.size() == bufferSize) {
                func.addTask({TaskType::print, figures});
                std::unique_lock<std::mutex> lock(readMutex);
                while(!done) {
                    var.wait(lock);
                }
                done = false;
                figures.resize(0);
            }
        } else {
            std::cout << "Unknown command" << std::endl;
        }    
    }
    thread.join();
    return 0;
}
