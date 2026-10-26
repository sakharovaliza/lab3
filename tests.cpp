#include <iostream>
#include <cassert>
#include <pthread.h>
#include "marker.h"

void TestFunc(int id, int n) {
    int* arr = new int[n];
    for (int i = 0; i < n; i++) arr[i] = 0;

    pthread_mutex_t cs;
    pthread_mutex_init(&cs, nullptr);
    Event hStartEvent(true);

    ThreadInfo info ={
        id, n, arr, &cs,
        &hStartEvent,
        new Event(false),
        new Event(false),
        new Event(false),
        false
    };

    pthread_t hThread;
    pthread_create(&hThread, nullptr, markerFunc, (void*)&info);
    hStartEvent.Set();

    info.hCantWorkEvent->Wait();

    for (int i = 0; i < n; i++) {
        assert((arr[i] == 0 || arr[i] == id) && "Arrays must be valid");
    }
    std::cout << "[OK] Test for marker " << id << " passed!\n";

    info.hStopEvent->Set();
    pthread_join(hThread, nullptr);

    for (int i = 0; i < n; i++) {
        assert(arr[i] == 0 && "Array must be cleared after stop");
    }

    delete info.hStopEvent;
    delete info.hCantWorkEvent;
    delete info.hMainContinueEvent;
    pthread_mutex_destroy(&cs);
    delete[] arr;
}

int main() {
    std::cout << "Running Unit Tests \n";
    TestFunc(1, 10);
    TestFunc(2, 10);
    TestFunc(3, 20);
    std::cout << "All tests passed! \n";
    return 0;
}