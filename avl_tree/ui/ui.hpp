#pragma once

//-----------------------------------------------------------------------------------------

#include <vector>
#include <set>
#include "graphviz.h"
#include "debug_utils.hpp"
#include "time_control.hpp"
#include "avl_tree.hpp"

//-----------------------------------------------------------------------------------------

namespace avl_tree_ui {

using namespace time_control;

void run_tree(std::istream & in_strm = std::cin);
void run_set_and_tree(std::istream & in_strm = std::cin);
void run_set(std::istream & in_strm = std::cin);

template<typename T, typename key_type>
static size_t range_query(const T& container, key_type l_bound, key_type u_bound) {
    using iter = typename T::iterator;

    iter start = container.lower_bound(l_bound);
    iter end   = container.upper_bound(u_bound);
    size_t res = std::distance(start, end);

    return res;
}

};

