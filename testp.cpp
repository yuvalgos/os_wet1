#include <iostream>
#include <chrono>
#include <thread>


int main() {
    for(int i = 0; i < 30; i++)
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(1000));
        //std::cout << "hello world" << std::endl;
    }
    
    return 0;
}