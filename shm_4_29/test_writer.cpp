#include "shm_writer.h"
#include <iostream>
#include <thread>
#include <chrono>

int main(){
    ShmWriter writer;

    if(!writer.init()){
        std::cerr << "[Writer] init failed" << std::endl;
        return -1;
    }
    std::cout << "[Writer] init success" << std::endl;

    uint8_t frame_data[FRAME_MAX_SIZE] = {0};
    for(int i =0; i<100; ++i){
        frame_data[i] = i;
    }

    uint64_t timestamp =0;
    while (true)
    {
        VideoFrameHead head;
        head.timestamp = timestamp++;
        head.len = FRAME_MAX_SIZE;
        head.width = 1920;
        head.height = 1080;
        head.type = 1;

        if(writer.write_frame(head, frame_data)){
            std::cout << "[Writer] write frame success | ts: " 
                     << head.timestamp 
                     << " | frame len: "
                     << head.len << std::endl;
        }
    }
    
}