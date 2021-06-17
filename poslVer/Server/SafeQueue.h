#pragma once
#pragma once
#ifndef SAFE_QUEUE
#define SAFE_QUEUE

#include <condition_variable>
#include <mutex>
#include <queue>

// A threadsafe-queue.
template <class T>
class SafeQueue
{
public:
    SafeQueue() : m_queue(), m_getElem__mutex(), m_getElem_cv(), m_isEmpty_mutex(), m_isEmpty_cv() {};

    ~SafeQueue() {};

    // Add an element to the queue.
    void enqueue(T t)
    {
        std::unique_lock<std::mutex> ul(m_isEmpty_mutex);
        std::lock_guard<std::mutex> lock(m_getElem__mutex);
        m_queue.push(t);
        m_isEmpty_ready = true;
        m_getElem_cv.notify_one();
        ul.unlock();
        m_isEmpty_cv.notify_one();
    }

    // Get the front element.
    // If the queue is empty, wait till a element is avaiable.
    T dequeue(void)
    {

        std::unique_lock<std::mutex> ul(m_isEmpty_mutex);
        m_isEmpty_cv.wait(ul, [this]() { return this->m_isEmpty_ready;});
        // std::cout << "NIJE BEZVEZE CEKAO" << std::endl;
        std::unique_lock<std::mutex> lock(m_getElem__mutex);
        // while (m_queue.empty())
        // {
             // release lock as long as the wait and reaquire it afterwards.
        //     m_getElem_cv.wait(lock);
       //  }
        T val = m_queue.front();
        m_queue.pop();
        if (m_queue.empty())
            m_isEmpty_ready = false;
        ul.unlock();
        return val;
    }


private:
    std::queue<T> m_queue;
    mutable std::mutex m_getElem__mutex;
    std::condition_variable m_getElem_cv;
    bool m_isEmpty_ready = false;
    std::mutex m_isEmpty_mutex;
    std::condition_variable m_isEmpty_cv;
};
#endif
