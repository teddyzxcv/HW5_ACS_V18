//
// Created by ZhengWu Pan on 15.12.2021.
//

#include "Call.h"

int Call::call_count_ = 0;

std::queue<pthread_t> *Call::calls_queue = new std::queue<pthread_t>();

// Constructor.
Call::Call(Talker *talker_1, Talker *talker_2) {
    this->caller = talker_1;
    this->receiver = talker_2;
}

// Rethink function for thread.
void *Call::waitToRethink(void *arg) {
    Talker *talker = (Talker *) arg;
    pthread_mutex_lock(&(talker->object_mutex));
    int buffer_talk_count = talker->take_call_count_;
    pthread_mutex_unlock(&(talker->object_mutex));
    int time = rand() % 5 + 1;
    sleep(time);
    pthread_mutex_lock(&(talker->object_mutex));
    if (buffer_talk_count == talker->take_call_count_) {
        talker->is_waiting_ = !talker->is_waiting_;
    }
    pthread_mutex_unlock(&(talker->object_mutex));
    if (!talker->is_waiting_) {
        Talker::call(talker);
    } else {
        waitToRethink(arg);
    }
}