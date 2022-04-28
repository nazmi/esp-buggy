#ifndef DEBUG_H
#define DEBUG_H

#include <cstdio>

// #define DEBUG
// #define SENSORS_DEBUG

#ifdef DEBUG
#define _DEBUG 1
#else
#define _DEBUG 0
#endif

#define DEBUGLOG(x, ...) \
    do { if (_DEBUG) { \
    printf("[LOG] %s %s(): " x, \
    __FILE__, __func__, ##__VA_ARGS__); \
    fflush(stdout); }} while (0)

#define ERROR(...) \
    do { if (_DEBUG) { \
    fprintf(stderr, "[ERROR] %s %s(): %s\n", \
    __FILE__, __func__, ##__VA_ARGS__); \
    fflush(stdout); __builtin_abort(); \
    }} while (0)

#endif