// ConsoleApplication1.cpp : This file contains the 'main' function. Program execution begins and ends there.
//
#include <iostream>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <chrono>

std::mutex mtx;                 // Mutex lock
std::condition_variable cond_var; // Condition variable
int shared_data = 0;             // Shared variable
bool data_ready = false;         // Boolean to indicate data availability

void write_thread() {
    for (int i = 1; i <= 10; i++) {
        std::unique_lock<std::mutex> lock(mtx); // Lock the mutex
        shared_data = i;  // Write to shared variable
        data_ready = true; // Data is ready
        std::cout << "Thread_1 wrote: " << shared_data << std::endl;
        cond_var.notify_one(); // Signal the reader thread
        lock.unlock(); // Unlock the mutex
        std::this_thread::sleep_for(std::chrono::seconds(1)); // Sleep, processing time
    }
}

void read_thread() {
    for (int i = 1; i <= 10; i++) {
        std::unique_lock<std::mutex> lock(mtx); // Lock the mutex
        cond_var.wait(lock, [] { return data_ready; }); // Wait until data is ready
        std::cout << "Thread_2 read: " << shared_data << std::endl;
        data_ready = false; // Reset flag
    }
}

int main() {
    std::thread writer(write_thread);
    std::thread reader(read_thread);

    writer.join();
    reader.join();

    return 0;
}

