// Header guard to prevent multiple inclusions of this header file
#ifndef THREAD_POOL_HPP
#define THREAD_POOL_HPP

#include <vector>              // for storing threads
#include <queue>               // for task queue
#include <thread>              // for std::thread
#include <mutex>               // for std::mutex and std::unique_lock
#include <future>              // for std::future and std::packaged_task
#include <functional>          // for std::function and std::bind
#include <condition_variable>  // for thread synchronization
#include <stdexcept>           // for std::runtime_error

class ThreadPool {
public:
    ThreadPool(size_t);   // Constructor: creates a pool with given number of threads
    ~ThreadPool();        // Destructor: joins all threads and cleans up

    // Enqueue a task to be run asynchronously in the thread pool
    // Returns a future representing the result of the task
    template<class F, class... Args>
    auto enqueue(F&& f, Args&&... args)
        -> std::future<typename std::invoke_result_t<F, Args...>>;

private:
    std::vector<std::thread> workers;                  // Vector of worker threads
    std::queue<std::function<void()>> tasks;           // Task queue

    std::mutex queueMutex;                             // Mutex for synchronizing access to the queue
    std::condition_variable condition;                 // Condition variable for task notification
    bool stop;                                         // Flag to stop the pool
};

// Template method must be defined in the header or an included .tpp file
template<class F, class... Args>
auto ThreadPool::enqueue(F&& f, Args&&... args)
    -> std::future<typename std::invoke_result_t<F, Args...>>
{
    using return_type = typename std::invoke_result_t<F, Args...>;  // Determine return type of the function

    // Wrap the task using packaged_task so we can get a future
    auto task = std::make_shared<std::packaged_task<return_type()>>(
        std::bind(std::forward<F>(f), std::forward<Args>(args)...)
    );

    std::future<return_type> res = task->get_future();  // Get the future from the packaged task

    {
        std::unique_lock<std::mutex> lock(queueMutex);  // Lock the queue mutex

        // If the pool has been stopped, throw an exception
        if (stop)
            throw std::runtime_error("enqueue on stopped ThreadPool");

        // Add the task to the queue. The lambda captures the task and invokes it.
        tasks.emplace([task]() { (*task)(); });
    }

    condition.notify_one();  // Notify one thread that a new task is available
    return res;              // Return the future to the caller
}

#endif // THREAD_POOL_HPP
