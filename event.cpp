#include "event.h"

Event::Event(bool manualReset): signaled_(false), manualReset_(manualReset){
    pthread_mutex_init(&mutex_, nullptr);
    pthread_cond_init(&cond_, nullptr);
}

Event::~Event(){
    pthread_mutex_destroy(&mutex_);
    pthread_cond_destroy(&cond_);
}

void Event::Set(){
    pthread_mutex_lock(&mutex_);
    signaled_ = true;
    if (manualReset_){
        pthread_cond_broadcast(&cond_);
    } else{
        pthread_cond_signal(&cond_);
    }
    pthread_mutex_unlock(&mutex_);
}

void Event::Reset(){
    pthread_mutex_lock(&mutex_);
    signaled_ = false;
    pthread_mutex_unlock(&mutex_);
}

void Event::Wait(){
    pthread_mutex_lock(&mutex_);
    while (!signaled_){
        pthread_cond_wait(&cond_, &mutex_);
    }
    if (!manualReset_){
        signaled_ = false;
    }
    pthread_mutex_unlock(&mutex_);
}

bool Event::IsSignaled(){
    pthread_mutex_lock(&mutex_);
    bool s = signaled_;
    pthread_mutex_unlock(&mutex_);
    return s;
}