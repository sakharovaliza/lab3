#include <iostream>
#include <vector>
#include <pthread.h>
#include "marker.h"
int main(){
    int n;
    std::cout << "Enter array size:";
    std::cin >> n;
    int* arr = new int[n];
    for (int i = 0; i<n; i++){
        arr[i]=0;
    }
    int markerCount;
    std::cout << "Enter number of markers:";
    std::cin >> markerCount;

    pthread_mutex_t cs;
    pthread_mutex_init(&cs, nullptr);

    Event* hStartEvent = new Event(true);

    std::vector<ThreadInfo*> infos(markerCount);
    std::vector<pthread_t> hThreads(markerCount);

    for (int i = 0; i < markerCount; ++i) {
        infos[i] = new ThreadInfo;
        infos[i]->id = i+1;
        infos[i]->arraySize = n;
        infos[i]->arr = arr;
        infos[i]->cs = &cs;
        infos[i]->hStartEvent = hStartEvent;
        infos[i]->hStopEvent = new Event(false);
        infos[i]->hCantWorkEvent = new Event(false);
        infos[i]->hMainContinueEvent = new Event(false);
        infos[i]->isTerminated = false;
        pthread_create(&hThreads[i], nullptr, markerFunc, (void*)infos[i]);
    }

    hStartEvent->Set();

    int activeMarkers = markerCount;
    while (activeMarkers > 0){
        for (int i = 0; i < markerCount; ++i) {
            if (!infos[i]->isTerminated) {
                infos[i]->hCantWorkEvent->Wait();
            }
        }

        std::cout << "\nCurrent array state:";
        for (int i = 0; i < n; ++i) {
            std::cout << arr[i] << " ";
        }
        std::cout << std::endl;

        int killId;
        bool found = false;
        while (!found) {
            std::cout << "Enter marker ID to stop:";
            std::cin >> killId;
            if (killId > 0 && killId <= markerCount && !infos[killId - 1]->isTerminated) {
                found = true;
            } else {
                std::cout << "Ti chto prikolist? Invalid ID or marker already stopped." << std::endl;
            }
        }

        infos[killId - 1]->hStopEvent->Set();
        pthread_join(hThreads[killId - 1], nullptr);
        infos[killId - 1]->isTerminated = true;
        activeMarkers--;

        std::cout << "Array after marker #" << killId << " stopped: ";
        for (int i = 0; i < n; ++i) {
            std::cout << arr[i] << " ";
        }
        std::cout << std::endl;

        for (int i = 0; i < markerCount; ++i) {
            if (!infos[i]->isTerminated) {
                infos[i]->hMainContinueEvent->Set();
            }
        }
    }
    for (int i = 0; i < markerCount; ++i) {
        delete infos[i]->hStopEvent;
        delete infos[i]->hCantWorkEvent;
        delete infos[i]->hMainContinueEvent;
        delete infos[i];
    }
    delete hStartEvent;
    pthread_mutex_destroy(&cs);
    delete[] arr;
    std::cout << "\nAll markers finished\n" << std::endl;
    return 0;
}