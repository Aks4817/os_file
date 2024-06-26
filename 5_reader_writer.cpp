#include <iostream>
#include <thread>
#include <mutex>
#include <vector>
#include <chrono>

// Constants defining the number of reader and writer threads
const int NUM_READERS = 3;
const int NUM_WRITERS = 2;

// Mutex for protecting the critical section during writes
std::mutex mtx;

// Mutex for reader count and ensuring first reader acquires write lock
std::mutex r_lock;

// Variable to track the number of readers currently reading
int readers_count = 0;

// Function for the reader threads
void reader(int readerID) {
  while (true) {
    // Begin reading section (acquire read lock)
    r_lock.lock();
    readers_count++;
    if (readers_count == 1) {  // First reader acquires write lock
      mtx.lock();
    }
    r_lock.unlock();

    // Simulate reading process
    std::cout << "Reader " << readerID << " is reading" << std::endl;
    std::this_thread::sleep_for(std::chrono::seconds(1));

    // End reading section (release read lock and potentially write lock)
    r_lock.lock();
    readers_count--;
    if (readers_count == 0) {  // Last reader releases write lock
      mtx.unlock();
    }
    r_lock.unlock();

    // Additional actions after reading (sleep for simulation)
    std::this_thread::sleep_for(std::chrono::seconds(1));
  }
}

// Function for the writer threads
void writer(int writerID) {
  while (true) {
    // Begin writing section (acquire write lock)
    mtx.lock();

    // Simulate writing process
    std::cout << "Writer " << writerID << " is writing" << std::endl;
    std::this_thread::sleep_for(std::chrono::seconds(2));

    // End writing section (release write lock)
    mtx.unlock();

    // Additional actions after writing (sleep for simulation)
    std::this_thread::sleep_for(std::chrono::seconds(1));
  }
}

int main() {
  // Vector to store reader threads
  std::vector<std::thread> reader_threads;

  // Vector to store writer threads
  std::vector<std::thread> writer_threads;

  // Create reader threads
  for (int i = 0; i < NUM_READERS; ++i) {
    reader_threads.push_back(std::thread(reader, i));
  }

  // Create writer threads
  for (int i = 0; i < NUM_WRITERS; ++i) {
    writer_threads.push_back(std::thread(writer, i));
  }

  // Wait for reader threads to finish
  for (auto& thread : reader_threads) {
    thread.join();
  }

  // Wait for writer threads to finish
  for (auto& thread : writer_threads) {
    thread.join();
  }

  return 0;
}
