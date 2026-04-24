#include "block_queue_4_23.cpp"

std::mutex cout_mutex;
int main(){
    BlockingQueue<int> queue(3);
    std::vector<std::thread> producers;
    std::vector<std::thread> consumers;
    for(int i=0; i<3; ++i){
        producers.emplace_back([&queue, i](){
            for(int j=0; j<3; ++j){
                int data = i*100+j;
                if(queue.try_push(data))
                {
                    std::lock_guard lock(cout_mutex);
                    std::cout << "push queue " << data << std::endl;
                }
                else 
                {
                    std::cout << "try push failed" << std::endl;
                    std::cout << "wait for push" << std::endl;
                    if(queue.push(data))
                    {
                        std::cout << "push queue " << std::endl;
                    }else{
                        std::cout << "push timeout " << std::endl;
                    }
                }
            }

        });
    }

    for(int i=0; i<3; ++i){
        consumers.emplace_back([&queue, i](){
            for(int j=0; j<3; ++j){
                int data;
                if(queue.try_pop(data))
                {                
                    std::lock_guard lock(cout_mutex);
                    std::cout << "pop queue " << data << std::endl;
                }
                else 
                {
                    std::cout << "try pop failed" << std::endl;
                    std::cout << "wait for pop" << std::endl;
                    if(queue.pop(data)){
                        std::cout << "pop queue " << data << std::endl;
                    }else {
                        std::cout << "pop timeout " << std::endl;
                    }
                }          
            }
        });
    }

    

    for(auto& t : producers) t.join();
    for(auto& t : consumers) t.join();
    std::cout << " all finished !" << std::endl;
    return 0;
}
