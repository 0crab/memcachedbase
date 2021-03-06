//
// Created by Michael on 10/27/19.
//

#include <iostream>
#include <cstring>
#include <pthread.h>
#include <memcached.h>
#include "tracer.h"

uint64_t total_count = (1 << 24);

int thread_number = 2;

memcached_server_st *servers = memcached_servers_parse("localhost");
memcached_st *memc = memcached_create(NULL);

void *measureWorker(void *args) {
    int tid = *(int *) args;
    char key[64];
    char value[64];
    Tracer tracer;
    tracer.startTime();
    memcached_behavior_set(memc, MEMCACHED_BEHAVIOR_BINARY_PROTOCOL, (uint64_t) tid);
    std::cout << "\tTid" << tid << ": " << tracer.getRunTime() << std::endl;
    for (int i = tid; i < total_count; i += thread_number) {
        std::memset(key, 0, 64);
        std::memset(value, 0, 64);
        std::sprintf(key, "%d", i);
        std::memcpy(value, "hello", 5);
        std::sprintf(value + 5, "%d", i);
        memcached_return_t ret = memcached_set(memc, key, std::strlen(key), value, std::strlen(value), 0, 0);
        if (tid == 0 && (i % 100000 == 0)) cout << "\t" << tid << " " << i << " " << ret << endl;
    }
}

int main(int argc, char **argv) {
    if (argc > 2) {
        total_count = std::atol(argv[1]);
        thread_number = std::atoi(argv[2]);
    }
    memcached_server_push(memc, servers);
    //memcached_server_list_free(servers);
    Tracer tracer;
    tracer.startTime();
    pthread_t threads[thread_number];
    int tids[thread_number];
    for (int i = 0; i < thread_number; i++) {
        tids[i] = i;
        pthread_create(&threads[i], nullptr, measureWorker, tids + i);
    }
    for (int i = 0; i < thread_number; i++) {
        pthread_join(threads[i], nullptr);
    }
    cout << total_count << "<->" << tracer.getRunTime() << endl;
    memcached_free(memc);
    return 0;
}