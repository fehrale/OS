// #include <iostream>
// #include <chrono>
// #include <iomanip>
// #include <thread>

// using namespace std::chrono_literals;

// int main()
// {
//     auto now = std::chrono::system_clock::now();
//     std::time_t now_time = std::chrono::system_clock::to_time_t(now);
//     std::tm local_tm = *std::localtime(&now_time);
//     std::cout << std::put_time(&local_tm, "%H:%M:%S - start\n");

//     std::this_thread::sleep_for(10s);

//     now = std::chrono::system_clock::now();
//     now_time = std::chrono::system_clock::to_time_t(now);
//     local_tm = *std::localtime(&now_time);
//     std::cout << std::put_time(&local_tm, "%H:%M:%S - end\n");
// }
#include <iostream>
#include <chrono>
#include <iomanip>
#include <thread>
#include "../include/timer_job.h"

void run_timer_job() {
    auto now = std::chrono::system_clock::now();
    std::time_t now_time = std::chrono::system_clock::to_time_t(now);
    std::tm local_tm = *std::localtime(&now_time);
    std::cout << std::put_time(&local_tm, "%H:%M:%S - start\n");

    std::this_thread::sleep_for(std::chrono::seconds(10));

    now = std::chrono::system_clock::now();
    now_time = std::chrono::system_clock::to_time_t(now);
    local_tm = *std::localtime(&now_time);
    std::cout << std::put_time(&local_tm, "%H:%M:%S - end\n");
}