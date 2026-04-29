#pragma once
#include "shm_common.h"
#include <sys/ipc.h>
#include <sys/shm.h>
class ShmReader
{
private:
    int32_t m_shmid = -1;
    ShmCtrlBlock* m_shm_ptr = nullptr;
    bool m_inited = false;
public:
    ShmReader(/* args */);
    ~ShmReader();

    ShmReader(const ShmReader&) = delete;
    ShmReader& operator=(const ShmReader&) = delete;

    bool init();
    bool read_frame(VideoFrameHead& out_head, uint8_t* out_data, uint32_t buf_size);

};

