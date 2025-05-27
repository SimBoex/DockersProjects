#include "thread_pool.hpp"

// Constructor: initializes the thread pool with the given number of threads
ThreadPool::ThreadPool(size_t threads)
    : stop(false) // Initially, do not stop the thread pool
{
    for (size_t i = 0; i < threads; ++i) {
        // Create and store each worker thread
        workers.emplace_back([this] {
            while (true) {
                std::function<void()> task;

                {
                    // Lock the task queue
                    std::unique_lock<std::mutex> lock(this->queueMutex);

                    // Wait until there's a task or the pool is stopping
                    this->condition.wait(lock, [this] {
                        return this->stop || !this->tasks.empty();
                    });
    
                    // Exit the loop if stopping and no tasks remain
                    if (this->stop && this->tasks.empty())
                        return;

                    // Get the next task from the queue
                    task = std::move(this->tasks.front());
                    this->tasks.pop();
                }

                // Execute the task outside the locked region
                task();
            }
        });
    }
}

// Destructor: stops all threads and cleans up
ThreadPool::~ThreadPool() {
    {
        // Lock the queue to set stop flag safely
        std::unique_lock<std::mutex> lock(queueMutex);
        stop = true; // Indicate to threads that the pool is shutting down
    }

    // Wake up all threads so they can exit
    condition.notify_all();

    // Join (wait for) each thread to finish
    for (std::thread &worker : workers)
        worker.join();
}
