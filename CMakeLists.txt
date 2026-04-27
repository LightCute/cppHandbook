cmake_minimum_required(VERSION 3.14)
project(cppHandbook LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})

find_package(Threads REQUIRED)

# ---- test_block_queue ----
add_executable(test_block_queue
    block_queue/test_block_queue.cpp
)
target_include_directories(test_block_queue PRIVATE block_queue)
target_link_libraries(test_block_queue PRIVATE Threads::Threads)

# ---- test_thread_pool ----
add_executable(test_thread_pool
    ThreadPool/test_thread_pool.cpp
)
target_include_directories(test_thread_pool PRIVATE ThreadPool block_queue)
target_link_libraries(test_thread_pool PRIVATE Threads::Threads)

# ---- test_shm_reader ----
add_executable(test_shm_reader
    shm/test_read/test_shm_reader.cpp
    shm/shm_reader.cpp
    shm/shm_common.cpp
)
target_include_directories(test_shm_reader PRIVATE shm)
target_link_libraries(test_shm_reader PRIVATE Threads::Threads)

# ---- test_shm_writer ----
add_executable(test_shm_writer
    shm/test_write/test_shm_writer.cpp
    shm/shm_writer.cpp
    shm/shm_common.cpp
)
target_include_directories(test_shm_writer PRIVATE shm)
target_link_libraries(test_shm_writer PRIVATE Threads::Threads)
