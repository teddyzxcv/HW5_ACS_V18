//
// Created by ZhengWu Pan on 13.12.2021.
//
#include <pthread.h>
#include <unistd.h>
#include <iostream>
#include <queue>
#include <list>
#include "string"

#ifndef HW5_ACS_V18_TALKER_H
#define HW5_ACS_V18_TALKER_H


class Talker {
public:
    Talker(std::string name);

    static pthread_mutex_t mutex;

    pthread_mutex_t object_mutex;

    static std::list<Talker *> *telephone_book_;

    static void *talk(void *arg);

    static void *printList(void *arg);

    static void call(Talker *caller);

    bool isAvailable();

    bool is_talking_;

    bool is_waiting_;

    std::string name_;

    uint take_call_count_;

private :
    static void createWaitThread(Talker *talker);

};


#endif //HW5_ACS_V18_TALKER_H
