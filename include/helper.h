#ifndef HELPER_H
#define HELPER_H

#include "encoder.h"
#include "mbed.h"
#include <functional>
#include <cstdio>

#ifdef DEBUG
#define _DEBUG 1
#else
#define _DEBUG 0
#endif

#define DEBUGLOG(x,...) \
    do { if(_DEBUG) { \
    printf("[LOG] %s %s(): " x, \
    __FILE__,__func__,##__VA_ARGS__ ); \
    fflush(stdout); }} while (0)

#define ERROR(...) \
    do { if(_DEBUG) { \
    fprintf(stderr,"[ERROR] %s %s(): %s\n", \
    __FILE__,__func__,##__VA_ARGS__ ); \
    fflush(stdout);__builtin_abort (); \
    }} while (0)

/**
 * @brief Clamp the value between [lo,high].
 *
 * @tparam T Type to work with. This project use mostly float.
 * @param v Original value.
 * @param lo Lower bound.
 * @param hi Upper bound.
 * @return constexpr const T& Clamped value.
 */
template <class T>
constexpr const T &clamp(const T &v, const T &lo, const T &hi) {
    return std::less<T>{}(v, lo) ? lo : std::less<T>{}(hi, v) ? hi
                                                              : v;
}

/**
 * @brief Convert m/s to pulse per sec(PPS).
 *
 * @tparam T Type to work with. This project use mostly double.
 * @param speed original speed.
 * @return constexpr const T converted pps.
 */
template <class T>
constexpr const T speed_to_pps(const T &speed) {
    return speed * 512 / Encoder::CIRCUMFERENCE;
}

/**
 * @brief Convert radians to degree.
 *
 * @tparam T Type to work with. This project use mostly double.
 * @param angle angle in radians
 * @return constexpr const T angle in degrees
 */
template <class T>
constexpr const T radiansToDegrees(const T &angle) {
    return angle * 180.0 / M_PI;
}

#endif
