#include <iostream>

#include "pubsub.hpp"
#include "figure.hpp"
#include "rhombus.hpp"
#include "hexagon.hpp"
#include "pentagon.hpp"


class ThreadFunc {
public:
    ThreadFunc(const TaskChanel& taskChanel) : taskChanel(taskChanel) {};

    void addTask(const Task& task) {
        std::lock_guard<std::mutex> lock(queueMutex);
        tasks.push(task);
    }

    void startWorking() {
        working = true;
    }

    void stopWorking() {
        working = false;
    }

    bool isWorking() {
        return working;
    }

    std::condition_variable& getVar1() {
        return var1;
    }
    
    std::condition_variable& getVar2() {
        return var2;
    }

    std::mutex& getReadMutex() {
        return readMutex;
    }

    void operator()() {
        while(true) {
            std::unique_lock<std::mutex> mainLock(readMutex);
            while(!working) {
                var2.wait(mainLock);
            }
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
                    this->stopWorking();
                    var1.notify_one();
                }
            }

        }
    }
private:
    TaskChanel taskChanel;
    std::mutex readMutex;
    std::condition_variable var1;
    std::condition_variable var2;
    std::mutex queueMutex;
    std::queue<Task> tasks;
    bool working = false;
};

int main(int argc, char** argv) {
    unsigned bufferSize;
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
            func.startWorking();
            func.getVar2().notify_one();
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
                func.startWorking();
                func.getVar2().notify_one();
                std::unique_lock<std::mutex> lock(func.getReadMutex());
                while(func.isWorking()) {
                    func.getVar1().wait(lock);
                }
                figures.resize(0);
            }
        } else {
            std::cout << "Unknown command" << std::endl;
        }    
    }
    thread.join();
    return 0;
}
