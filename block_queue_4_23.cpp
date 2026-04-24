#include <iostream>
#include <queue>
#include <mutex>
#include <condition_variable>
#include <vector>
#include <thread>
#include <atomic>
template<typename T>
class BlockingQueue{
private: 
    std::queue<T> queue_;
    mutable std::mutex mutex_;
    std::condition_variable cv_producer_;
    std::condition_variable cv_consumer_;
    size_t size_max_;
    std::atomic<bool> closed_{false};
public:
    BlockingQueue(size_t size_max) : size_max_(size_max){}
    ~BlockingQueue() = default;

    void close(){
        std::lock_guard<std::mutex> lock(mutex_);
        closed_ = true;
        cv_producer_.notify_all();
        cv_consumer_.notify_all();
    }

    bool push(const T& val){
        std::unique_lock<std::mutex> lock(mutex_);
        cv_producer_.wait(lock, 
            [this](){return closed_ || (queue_.size() < size_max_);});
        if(closed_) return false;
        queue_.push(val);
        cv_consumer_.notify_one();
        return true;
    }

    bool push(T&& val){
        std::unique_lock<std::mutex> lock(mutex_);
        cv_producer_.wait(lock, 
            [this](){return closed_ || (queue_.size() < size_max_);});
        if(closed_) return false;
        queue_.push(std::move(val));
        cv_consumer_.notify_one();
        return true;
    }

    bool try_push(const T& val){
        std::lock_guard<std::mutex> lock(mutex_);
        if(queue_.size() >= size_max_) return false;
        queue_.push(val);
        cv_consumer_.notify_one();
        return true;
    }

    bool try_push(T&& val){
        std::lock_guard<std::mutex> lock(mutex_);
        if(queue_.size() >= size_max_) return false;
        queue_.push(std::move(val));
        cv_consumer_.notify_one();
        return true;
    }

    bool pop(T& val){
        std::unique_lock<std::mutex> lock(mutex_);
        cv_consumer_.wait(lock, 
            [this](){
            return closed_ || !queue_.empty();});
        if(closed_ && queue_.empty()) return false;
        val = queue_.front();
        queue_.pop();
        cv_producer_.notify_one();
        return true;
    }

    bool try_pop(T& val){
        std::lock_guard<std::mutex> lock(mutex_);
        if(queue_.empty()) return false;
        val = queue_.front();
        queue_.pop();
        cv_producer_.notify_one();
        return true;
    }

    size_t size(){
        std::lock_guard<std::mutex> lock(mutex_);
        return queue_.size();
    }

    bool empty() const {
        std::lock_guard<std::mutex> lock(mutex_);
        return queue_.empty();
    }

    bool full() const {
        std::lock_guard<std::mutex> lock(mutex_);
        return queue_.size() >= size_max_;
    }

    bool is_closed()  {
        std::lock_guard<std::mutex> lock(mutex_);
        return closed_;
    }

    void clear() {
        std::lock_guard<std::mutex> lock(mutex_);
        std::queue<T> empty_queue;
        std::swap(queue_, empty_queue);
    }

};
