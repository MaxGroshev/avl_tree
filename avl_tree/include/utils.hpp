#pragma once

#include <iostream>
#include <cmath>
#include "debug_utils.hpp"

//-----------------------------------------------------------------------------------------

template <typename T>
T find_max (const T x, const T y) {
    if (x > y) return x;
    return y;
}

static bool in_interval(const int l_bound, const int u_bound, const int val) {
    if (val >= l_bound && val <= u_bound) return true;
    return false;
}

//-----------------------------------------------------------------------------------------
