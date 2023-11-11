#include "ui.hpp"

//-----------------------------------------------------------------------------------------

int main() {
    using namespace avl_tree_ui;

    auto tree_start_time = time_control::chrono_cur_time ();
    avl_tree_ui::run_set();
    auto tree_end_time = time_control::chrono_cur_time ();

    std::clog << "----------------------------------------------\n";
    std::clog << "Total set run time: " << (tree_end_time - tree_start_time) / 0.1ms
    << " * 10^(-5) sec\n";

    return 0;
}

//-----------------------------------------------------------------------------------------

void avl_tree_ui::run_set(std::istream & in_strm) {
    std::set<int> enemy_set;
    char type_of_data = '\0';
    int data    = 0;
    int l_bound = 0;
    int u_bound = 0;

    while(!in_strm.eof()) {
        in_strm >> type_of_data;
        if (type_of_data == 'k') {
            in_strm >> data;
            enemy_set.insert(data);
        }
        else if (type_of_data == 'q') {
            in_strm >> l_bound >> u_bound;
            if (l_bound >= u_bound) {
                std::cout << 0 << ' ';
            }
            else {
                std::cout << avl_tree_ui::range_query(enemy_set, l_bound, u_bound) << ' ';
            }
        }
        type_of_data = '\0';
    }
    std::cout << std::endl;
}
