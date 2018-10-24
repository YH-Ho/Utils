#include <iostream>
#include <cstring>
#include <blockingqueue.h>
#include <thread>
#include <signal.h>

using namespace std;

static bool g_quit = false;

void *thread_1(BlockingQueue<int*> &queue) {
    int *value = nullptr;
    int i = 0;

    while (!g_quit) {
        i++;
        value = &i;
        queue.push(value);
        //cout<<"queue push: "<<*value<<" "<<"queue size: "<<queue.size()<<endl;
    }

    return nullptr;
}

void *thread_2(BlockingQueue<int*> &queue) {
    int i = 0;
    //int *value = nullptr;

    while (!g_quit) {
        /* for delay */
        if(++i % 100000000 == 0) {
            //queue.wait_and_pop(value);
            cout<<"queue pop: "<<**queue.wait_and_pop().get()<<" "<<"queue size: "<<queue.size()<<endl;
        }
    }

    queue.clear();
    cout<<"<Clear> queue size :"<<queue.size()<<endl;

    return nullptr;
}

void sig_handle(int arg) {
    g_quit = true;
}

int main(int argc, char**argv) {
    signal(SIGINT, sig_handle);

    BlockingQueue<int*> test_queue(1000);

    thread thd_1(thread_1,  ref(test_queue));
    thread thd_2(thread_2,  ref(test_queue));

    thd_1.join();
    thd_2.join();

    cout<<"exit"<<endl;
    return 0;
}
