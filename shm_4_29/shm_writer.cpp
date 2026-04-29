#include "shm_writer.h" 
#include <iostream>
#include <cstring>



ShmWriter::ShmWriter() = default;
ShmWriter::~ShmWriter(){
    if(m_shmid >= 0){
        m_shmid = shmctl(m_shmid, IPC_RMID, nullptr);
        m_shmid = -1;
    }
    if(m_shm_ptr){
        shmdt(m_shm_ptr);
        m_shm_ptr = nullptr;
    }
}


bool ShmWriter::init(){
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
    if(reinterpret_cast<void*>(m_shm_ptr) == reinterpret_cast<void*>(-1)){
        perror("shmat failed");
        return false;
    }

    new (m_shm_ptr) ShmCtrlBlock{};

    init_shm_sync(m_shm_ptr);
    m_inited = true;
    std::cout << "[ShmWriter] init success" << std::endl;
    return true;
}

bool ShmWriter::write_frame(const VideoFrameHead& head, uint8_t* data){
    if(!m_inited || !m_shm_ptr || !data)
        return false;
    pthread_mutex_lock(&(m_shm_ptr->mtx));

    while(m_shm_ptr->frame_count >= RING_MAX_COUNT){
        pthread_cond_wait(&(m_shm_ptr->cv_can_write), &(m_shm_ptr->mtx));
    }

    auto& item = m_shm_ptr->ring[m_shm_ptr->w_idx];
    item.head = head;
    std::memcpy(item.data, data, head.len);
    m_shm_ptr->w_idx = (m_shm_ptr->w_idx + 1) % RING_MAX_COUNT;
    m_shm_ptr->frame_count++;
    pthread_cond_signal(&(m_shm_ptr->cv_can_read));
    pthread_mutex_unlock(&(m_shm_ptr->mtx));
    return true;
}
