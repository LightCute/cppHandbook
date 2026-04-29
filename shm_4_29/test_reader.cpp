#include "shm_reader.h"
#include <iostream>
#include <thread>
#include <chrono>

int main(){
    ShmReader reader;
    if(!reader.init()){
        std::cerr << "[Reader] init failed" << std::endl;
        return -1;
    }
    std::cout << "[Reader] init success" << std::endl;
    uint8_t recv_data[FRAME_MAX_SIZE] = {0};
    VideoFrameHead recv_head;
    while (true)
    {
        if(reader.read_frame(recv_head, recv_data, FRAME_MAX_SIZE)){
            std::cout << "[Reader] read success "
                        <<  " | ts: " << recv_head.timestamp
                        <<  " | frame len: " << recv_head.len
                        << std::endl;
        }
        std::this_thread::sleep_for(std::chrono::microseconds(100));
    }
    return 0;
    
}