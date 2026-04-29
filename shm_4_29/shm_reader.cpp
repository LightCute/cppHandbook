#include "shm_reader.h"
#include <iostream>
#include <cstring>
ShmReader::ShmReader() = default;

ShmReader::~ShmReader()
{
    if(m_shmid >= 0){
        m_shmid = shmctl(m_shmid, IPC_RMID, nullptr);
        m_shmid = -1;
    }
}

bool ShmReader::init(){
    key_t key = ftok(SHM_KEY_PATH, SHM_KEY_ID);
    if(key == -1){
        perror("ftok failed");
        return false;
    }

    m_shmid = shmget(key, SHM_CTRL_BLOCK_SIZE, IPC_CREAT | 0666);
    if(m_shmid == -1){
        perror("shmget failed");
        return false;
    }

    m_shm_ptr = reinterpret_cast<ShmCtrlBlock*>(shmat(m_shmid, nullptr, 0));
    if (reinterpret_cast<void*>(m_shm_ptr) == reinterpret_cast<void*>(-1)){
        perror("shmat failed");
        return false;
    }

    m_inited = true;
    return true;

}

bool ShmReader::read_frame(VideoFrameHead& out_head, uint8_t* out_data, uint32_t buf_size){
    if(!m_inited || !m_shm_ptr || !out_data)
        return false;

    pthread_mutex_lock(&(m_shm_ptr->mtx));
    while (m_shm_ptr->frame_count <= 0)
    {
        pthread_cond_wait(&(m_shm_ptr->cv_can_read), &(m_shm_ptr->mtx));
    }
    auto& item = m_shm_ptr->ring[m_shm_ptr->r_idx];
    out_head = item.head;
    if(out_head.len <= buf_size){
        std::memcpy(out_data, item.data, out_head.len);
    }
    m_shm_ptr->r_idx = (m_shm_ptr->r_idx + 1) % RING_MAX_COUNT; 
    m_shm_ptr->frame_count--;
    pthread_cond_signal(&(m_shm_ptr->cv_can_write));
    pthread_mutex_unlock(&(m_shm_ptr->mtx));
    return true;
}

