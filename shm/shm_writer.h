//shm_writer.h
#pragma once
#include "shm_common.h"
#include <sys/ipc.h>
#include <sys/shm.h>

class ShmVideoWriter
{
private:
    int32_t m_shmid{-1};
    ShmCtrlBlock* m_shm_ptr{nullptr};
    bool m_inited{false}; 
public:
    ShmVideoWriter(/* args */);
    ~ShmVideoWriter();

    ShmVideoWriter(const ShmVideoWriter&) = delete;
    ShmVideoWriter& operator=(const ShmVideoWriter&) = delete;

    bool init();
    bool write_frame(const VideoFrameHead& head, const uint8_t* data);
};

