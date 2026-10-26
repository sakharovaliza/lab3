#include "marker.h"
#include <iostream>
#include <unistd.h>

void* markerFunc(void* pArguments){
    ThreadInfo* data = static_cast<ThreadInfo*>(pArguments);
    srand(data->id);
    data->hStartEvent->Wait();

    int counter = 0;
    while (true){
        int index = rand() % data->arraySize;

        pthread_mutex_lock(data->cs);

        if (data->arr[index] == 0){
            usleep(5000);
            data->arr[index] = data->id;
            counter++;
            usleep(5000);
            pthread_mutex_unlock(data->cs);
        } else{
            std::cout << "\nMarker: " << data->id << std::endl;
            std::cout << "Marked: " << counter << std::endl;
            std::cout << "Blocked at index: " << index << std::endl;

            pthread_mutex_unlock(data->cs);

            data->hCantWorkEvent->Set();
            while (true){
                if (data->hStopEvent->IsSignaled()) break;
                if (data->hMainContinueEvent->IsSignaled()) break;
                usleep(1000);
            }
            if (data->hStopEvent->IsSignaled()){
                pthread_mutex_lock(data->cs);
                for (int i = 0; i < data->arraySize; i++){
                    if (data->arr[i] == data->id){
                        data->arr[i] = 0;
                    }
                }
                pthread_mutex_unlock(data->cs);
                break;
            }
            data->hMainContinueEvent->Reset();
        }
    }
    return nullptr;
}