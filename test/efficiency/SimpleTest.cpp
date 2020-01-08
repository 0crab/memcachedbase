//
// Created by Michael on 10/27/19.
//

#include <iostream>
#include <cstring>
#include <memcached.h>
#include "tracer.h"

constexpr size_t total_count = 1000000;

int main(int argc, char **argv) {
    memcached_server_st *servers = memcached_servers_parse("localhost");
    memcached_st *memc = memcached_create(NULL);
    memcached_server_push(memc, servers);
    //memcached_server_list_free(servers);
    memcached_behavior_set(memc, MEMCACHED_BEHAVIOR_BINARY_PROTOCOL, (uint64_t) 0);
    char key[64];
    char value[64];
    size_t success = 0;
    Tracer tracer;
    tracer.startTime();
    for (int i = 0; i < total_count; i++) {
        std::memset(key, 0, 64);
        std::memset(value, 0, 64);
        std::sprintf(key, "%d", i);
        std::memcpy(value, "hello", 5);
        std::sprintf(value + 5, "%d", i);
        memcached_return_t ret = memcached_set(memc, key, std::strlen(key), value, std::strlen(value), 0, 0);
        if (ret == memcached_return_t::MEMCACHED_SUCCESS) success++;
    }
    cout << tracer.getRunTime() << "\t" << success << " " << total_count << endl;
    memcached_free(memc);
#if MEMCACINFO
    long *runtime = get_runtime();
    cout << runtime[0] << "\t" << runtime[1] << " " << get_count() << endl;
#endif
    return 0;
}