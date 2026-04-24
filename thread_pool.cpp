#include <functional>
#include <atomic>
#include <future>
#include "block_queue_4_23.cpp"



class ThreadPool{
private:
    using Task = std::function<void()>;
    BlockingQueue<Task> task_queue_;
    std::vector<std::thread> workers_;
    std::atomic<bool> is_running_;
public:
    ThreadPool(int thread_num)
        : task_queue_(10000), is_running_(true){
        for(int i=0; i<thread_num; ++i){
            workers_.emplace_back(&ThreadPool::work_loop, this);
        }
    }
    ThreadPool(const ThreadPool&) = delete;
    ThreadPool& operator=(const ThreadPool&) = delete;
    ~ThreadPool(){
        stop();
    }

    template<typename Func, typename... Args>
    auto submit(Func&& func, Args&&... args)
        -> std::future<decltype(func(args...))>
    {
        using RetType = decltype(func(args...));
        auto task = std::make_shared<std::packaged_task<RetType()>>(
            std::bind(std::forward<Func>(func), std::forward<Args>(args)...)
        );
        std::future<RetType> future = task->get_future();
        task_queue_.push([task](){(*task)(); });
        return future;
    }


private:

    void work_loop(){
        while (is_running_)
        {
            Task task;
            if(task_queue_.pop(task)){
                if(task){
                    task();
                }
            }
        }   
    }

    void stop(){
        is_running_ = false;
        
        for(auto& t : workers_) {
            if(t.joinable()){
                t.join();
            }
        }
    }
};

