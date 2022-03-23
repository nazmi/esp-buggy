#include "mbed.h"

template<class T>
constexpr const T& clamp( const T& v, const T& lo, const T& hi)
{
    return std::less<T>{}(v, lo) ? lo : std::less<T>{}(hi, v) ? hi : v;
}