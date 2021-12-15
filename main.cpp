#include <iostream>
#include <pthread.h>
#include "Talker.h"
#include "Call.h"
#include <stack>
#include "string"


int main() {
    std::cout << "Input talkers count:";
    std::cin >> Talker::talker_count_;
    for (int i = 0; i < Talker::talker_count_; ++i) {
        Talker *newTalker = new Talker("talker-" + std::to_string(i));
        Talker::telephone_book_->push_back(newTalker);
    }
    Talker::printList(NULL);
    for (Talker *caller: *Talker::telephone_book_) {
        if (!caller->is_talking_ && !caller->is_waiting_ && caller->isAvailable()) {
            Talker::call(caller);
        }
    }
    while (!Call::calls_queue->empty()) {
        void *is_run;
        pthread_mutex_lock(&(Talker::mutex));
        pthread_t call_thread = Call::calls_queue->front();
        pthread_mutex_unlock(&(Talker::mutex));
        pthread_join(call_thread, &is_run);
        pthread_mutex_lock(&(Talker::mutex));
        Call::calls_queue->pop();
        pthread_mutex_unlock(&(Talker::mutex));
    }
    Talker::printList(NULL);
}
