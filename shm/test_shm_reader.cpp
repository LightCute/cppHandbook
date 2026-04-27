//test_shm_reader.cpp
#include "shm_reader.h"
#include <iostream>

int main(){
    ShmVideoReader reader;
    if(!reader.init())
        return -1;
    uint8_t recv_buf[FRAME_MAX_SIZE] = {0};
    VideoFrameHead out_head{};

    while (true)    
    {
        reader.read_frame(out_head, recv_buf, FRAME_MAX_SIZE);
        std::cout 
            << "read frame | ts: " << out_head.timestamp
            << "w: " << out_head.width
            << "h: " << out_head.height
            << "len: " << out_head.frame_len << std::endl;
    }
    
    
    return 0;
}