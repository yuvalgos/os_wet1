#include <iostream>
#include <chrono>
#include <thread>


int main() {
    // for(int i = 0; i < 100; i++)
    while(1)
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(5000));
        std::cout << "hello world" << std::endl;
    }
    
    return 0;
}