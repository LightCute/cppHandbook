#include <cstdint>
#include <pthread.h>

inline constexpr int RING_MAX_COUNT = 8;
inline constexpr uint32_t FRAME_MAX_SIZE = 2 * 1024 * 1024;


struct VideoFrameHead   
{
    uint64_t timestamp; 
    uint32_t len;
    uint32_t width;
    uint32_t height;
    uint32_t type;

};


struct VideoFrame
{
    VideoFrameHead head;
    uint8_t data[FRAME_MAX_SIZE];
};


struct ShmCtrlBlock 
{
    pthread_mutex_t mtx;
    pthread_cond_t cv_can_write;
    pthread_cond_t cv_can_read;
    uint32_t w_idx;
    uint32_t r_idx;
    uint32_t frame_count;
    VideoFrame ring[RING_MAX_COUNT];
    ShmCtrlBlock() = default;
    ~ShmCtrlBlock() = default;
    ShmCtrlBlock(const ShmCtrlBlock&) = delete;
    ShmCtrlBlock& operator=(const ShmCtrlBlock&) = delete;
};

inline constexpr size_t SHM_CTRL_BLOCK_SIZE = sizeof(ShmCtrlBlock);
void init_shm_sync(ShmCtrlBlock* shm);
