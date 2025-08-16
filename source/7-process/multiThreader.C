#include <iostream>
#include <thread>
#include <vector>
#include <functional>
#include <mutex>
#include <condition_variable>
#include <memory>

class MultiThreaderGimn {
private:
    int maxThreads;
    std::vector<bool> Workers;  // Indicates if a worker is busy
    std::vector<std::shared_ptr<std::thread>> workersT;
    std::mutex mtx; // Mutex for thread safety
    std::condition_variable cv; // Condition variable for waiting
    int activeThreads = 0; // Count of active threads

public:
    MultiThreaderGimn(int maxThreads = 28) : maxThreads(maxThreads) {
        Workers.resize(maxThreads, false);
    }
    
    void createThread(std::function<void(std::vector<TString>)> func, std::vector<TString> files) {
        std::unique_lock<std::mutex> lock(mtx);

        // Wait until there is a free worker or we can create a new thread
        while (activeThreads >= maxThreads) {
            cv.wait(lock); // Wait until notified that a thread has finished
        }

        // Create a new thread
        auto workerThread = std::make_shared<std::thread>([this, func, files]() {
            func(files);
            // Mark the thread as finished
            std::unique_lock<std::mutex> lock(mtx);
            activeThreads--;
            cv.notify_one(); // Notify one waiting thread that a worker is free
        });

        workersT.push_back(workerThread);
        activeThreads++; // Increment the count of active threads

        // Detach the thread so it runs independently
        workerThread->detach();
    }

    void waitForCompletion() {
        std::unique_lock<std::mutex> lock(mtx);
        // Wait until all threads have finished
        cv.wait(lock, [this]() { return activeThreads == 0; });
    }
};

