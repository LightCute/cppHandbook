//test_shm_writer.cpp
#include "shm_writer.h" 
#include <iostream>
#include <unistd.h>

int main(){
    ShmVideoWriter writer;
    if(!writer.init()) return -1;
    uint8_t fake_frame[FRAME_MAX_SIZE] = {0};
    VideoFrameHead head{};
    head.width = 1920;
    head.height = 1080;
    head.frame_type = 1;

    while (true)
    {
        head.timestamp += 33;
        head.frame_len = 1024*512;
        if (writer.write_frame(head, fake_frame))
        {
            std::cout << "write a frame, ts: " << head.timestamp << std::endl;
            
        }else {
            std::cout << "write failed" << std::endl;
        }

        usleep(33000);
    }
    
    return 0;
}