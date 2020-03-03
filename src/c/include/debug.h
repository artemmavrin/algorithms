#ifndef __ALGORITHMS_DEBUG_H
#define __ALGORITHMS_DEBUG_H

#include <assert.h>
#include <stdio.h>

#ifndef DEBUG
#define DEBUG 0
#else
#undef DEBUG
#define DEBUG 1
#endif

// Printf-like variadic macro for writing debugging messages to stderr. The
// single iteration do loop and constant branch should be optimized away by the
// compiler
#define DPRINTF(...)                                                           \
    do {                                                                       \
        if (DEBUG) {                                                           \
            fprintf(stderr, "DEBUG %s:%d:%s: ", __FILE__, __LINE__, __func__); \
            fprintf(stderr, __VA_ARGS__);                                      \
        }                                                                      \
    } while (0)  // To force a semicolon

#endif
