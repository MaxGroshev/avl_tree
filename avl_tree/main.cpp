#include "ui.hpp"

//-----------------------------------------------------------------------------------------

int main() {
    using namespace avl_tree_ui;

    auto tree_start_time = time_control::chrono_cur_time ();
    avl_tree_ui::run_tree();
    auto tree_end_time = time_control::chrono_cur_time ();

    std::clog << "----------------------------------------------\n";
    std::clog << "Total tree run time: " << (tree_end_time - tree_start_time) / 0.1ms
    << " * 10^(-5) sec\n";

    return 0;
}
