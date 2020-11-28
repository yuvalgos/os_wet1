#include <iostream>
#include <chrono>
#include <thread>


int main() {
    // 
    //while(1)
    for(int i = 0; i < 20; i++)
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(7000));

        std::cout << "hello world" << std::endl;
    }
    
    return 0;
}