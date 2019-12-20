#pragma once

#include <filesystem>
#include <fstream>
#include <memory>
#include <vector>
#include <queue>
#include <map>
#include <thread>
#include <mutex>
#include <condition_variable>

#include "figure.hpp"

enum class TaskType {print, exit};

class Task {
public:
    Task(TaskType type, const std::vector<std::shared_ptr<Figure>>& data) : type(type), data(data) {};
    TaskType getType() const {
        return type;
    }
    std::vector<std::shared_ptr<Figure>> getData() const {
        return data;
    }
private:
    TaskType type;
    std::vector<std::shared_ptr<Figure>> data;
};

class Subscriber {
public:    
    virtual void print(const Task& task) const = 0;
    virtual ~Subscriber() = default;
};

class ConsolePrinter : public Subscriber {
public:    
    void print(const Task& task) const override {
        auto data = task.getData();
        for(const auto& figure : data) {
            figure->Print(std::cout);
        }      
    }
};

class FilePrinter : public Subscriber {
public:
    void print(const Task& task) const override {
        auto data = task.getData();
        std::ofstream os(std::to_string(rand() % 1337) + ".txt");
        if(os.bad()) {
            std::cout << "Couldn't open file\n"; 
        }
        for(const auto& figure : data) {
            figure->Print(os);
        }
    }
};


class TaskChanel {
public:
    void subscribe(std::shared_ptr<Subscriber>& s) {
        subscribers.push_back(s);
    }

    void notify(const Task& task) {
        for(const auto& subscriber : subscribers) {
            subscriber->print(task);
        }
    }
private:
    std::vector<std::shared_ptr<Subscriber>> subscribers;
};


