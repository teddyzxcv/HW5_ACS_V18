//
// Created by ZhengWu Pan on 15.12.2021.
//
#include "Talker.h"

#ifndef HW5_ACS_V18_CALL_H
#define HW5_ACS_V18_CALL_H


class Call {
public:
    Talker *caller;
    Talker *receiver;

    static int call_count_;

    static std::queue<pthread_t> *calls_queue;

    static void *waitToRethink(void *arg);

    Call(Talker *talker_1, Talker *talker_2);
};


#endif //HW5_ACS_V18_CALL_H
