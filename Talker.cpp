//
// Created by ZhengWu Pan on 13.12.2021.
//

#include "Talker.h"
#include "Call.h"


pthread_mutex_t Talker::mutex = PTHREAD_MUTEX_INITIALIZER;

std::list<Talker *> *Talker::telephone_book_ = new std::list<Talker *>();

int Talker::talker_count_ = 0;

Talker::Talker(std::string name) {
    this->object_mutex = PTHREAD_MUTEX_INITIALIZER;
    this->is_talking_ = false;
    this->take_call_count_ = 0;
    this->name_ = name;
    if (rand() & 1) {
        this->is_waiting_ = false;
    } else {
        this->is_waiting_ = true;
    }
}

void Talker::call(Talker *caller) {
    for (Talker *receiver: *telephone_book_) {
        pthread_mutex_lock(&mutex);
        bool is_receiver_available = receiver->isAvailable();
        bool is_caller_available = caller->isAvailable();
        pthread_mutex_unlock(&mutex);
        if (!receiver->is_talking_ & receiver != caller && is_receiver_available &&
            is_caller_available) {
            pthread_mutex_lock(&mutex);
            caller->is_talking_ = true;
            receiver->is_talking_ = true;
            caller->take_call_count_++;
            receiver->take_call_count_++;
            pthread_mutex_unlock(&mutex);
            Call *call = new Call(caller, receiver);
            pthread_t call_thread;
            pthread_create(&call_thread, NULL, Talker::talk, call);
            pthread_mutex_lock(&mutex);
            Call::calls_queue->push(call_thread);
            pthread_mutex_unlock(&mutex);
            return;
        }
    }
}

bool Talker::isAvailable() {
    bool is_available = this->take_call_count_ < 5;
    return is_available;
}

void *Talker::printList(void *arg) {
    pthread_mutex_lock(&mutex);
    for (Talker *talker: *telephone_book_) {
        printf("%s : call taken: %d, is available: %d, is waiting: %d, is talking: %d\n", talker->name_.c_str(),
               talker->take_call_count_,
               talker->isAvailable(), talker->is_waiting_, talker->is_talking_);
    }
    printf("\n");
    pthread_mutex_unlock(&mutex);
    return nullptr;
}

void Talker::createWaitThread(Talker *talker) {
    pthread_mutex_unlock(&(talker->object_mutex));
    pthread_t wait_thread;
    pthread_create(&wait_thread, NULL, Call::waitToRethink, talker);
    pthread_mutex_lock(&(talker->object_mutex));
    Call::calls_queue->push(wait_thread);
    pthread_mutex_unlock(&(talker->object_mutex));
}

void *Talker::talk(void *arg) {
    Call *call = (Call *) arg;
    pthread_mutex_lock(&(call->caller->object_mutex));
    pthread_mutex_lock(&(call->receiver->object_mutex));
    int current_call = Call::call_count_++;
    printf("Start call number %d, from %s to %s\n", current_call, call->caller->name_.c_str(),
           call->receiver->name_.c_str());
    pthread_mutex_unlock(&(call->caller->object_mutex));
    pthread_mutex_unlock(&(call->receiver->object_mutex));
    int time = rand() % 4 + 1;
    sleep(time);
    pthread_mutex_lock(&(call->caller->object_mutex));
    pthread_mutex_lock(&(call->receiver->object_mutex));
    call->caller->is_talking_ = false;
    call->receiver->is_talking_ = false;
    printf("End call, %d sec, call number %d\n", time, current_call);
    if (rand() & 1) {
        call->caller->is_waiting_ = false;
        createWaitThread(call->caller);
    } else {
        call->caller->is_waiting_ = true;
        createWaitThread(call->caller);
    }
    if (rand() & 1) {
        call->receiver->is_waiting_ = false;
        createWaitThread(call->receiver);
    } else {
        call->receiver->is_waiting_ = true;
        createWaitThread(call->receiver);
    }
    return (void *) true;
}

