#include <iostream>
#include <functional>
#include <thread>
#include <chrono>

class Timer {
    bool clear = false;

    public:
        void setTimeout(std::function<void()> func, int delay);
        void setInterval(std::function<void()> func, int interval);
        void stop();

};

void Timer::setTimeout(std::function<void()> func, int delay) {
    this->clear = false;
    std::thread t([=]() {
        if(this->clear) return;
        std::this_thread::sleep_for(std::chrono::milliseconds(delay));
        if(this->clear) return;
        func();
    });
    t.detach();
}

void Timer::setInterval(std::function<void()> func, int interval) {
    this->clear = false;
    std::thread t([=]() {
        while(true) {
            if(this->clear) return;
            std::this_thread::sleep_for(std::chrono::milliseconds(interval));
            if(this->clear) return;
            func();
        }
    });
    t.detach();
}

void Timer::stop() {
    this->clear = true;
}
