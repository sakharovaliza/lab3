#pragma once
#include <pthread.h>
#include "event.h"

struct ThreadInfo{
    int id;
    int arraySize;
    int* arr;
    pthread_mutex_t* cs;
    Event* hStartEvent;
    Event* hStopEvent;
    Event* hCantWorkEvent;
    Event* hMainContinueEvent;
    bool isTerminated;
};
void* markerFunc(void* pArguments);