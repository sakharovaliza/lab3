#pragma once
#include <pthread.h>
class Event{
public:
    Event(bool manualReset);
    ~Event();
    void Set();
    void Reset();
    void Wait();
    bool IsSignaled();
    pthread_mutex_t mutex_;
    pthread_cond_t cond_;
    bool signaled_;
    bool manualReset_;
};