
#include "shm_common.h"
#include <sys/shm.h>
#include <sys/ipc.h>
class ShmWriter
{
private:
    int32_t m_shmid = -1;
    ShmCtrlBlock* m_shm_ptr = nullptr;
    bool m_inited = false;
public:
    ShmWriter(/* args */);
    ~ShmWriter();

    ShmWriter(const ShmWriter&) = delete;
    ShmWriter& operator=(const ShmWriter&) = delete;

    bool init();
    bool write_frame(VideoFrameHead& head, uint8_t* data);

};


