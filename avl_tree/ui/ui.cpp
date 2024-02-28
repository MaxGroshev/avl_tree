#include "./ui.hpp"

//-----------------------------------------------------------------------------------------

namespace avl_tree_ui {

void run_tree(std::istream & in_strm) {
    avl::tree_t<int> pine;
    char type_of_data = '\0';
    int data    = 0;
    int l_bound = 0;
    int u_bound = 0;

    std::vector<size_t> result;
    while(!in_strm.eof()) { //^D to exit from cin input
        in_strm >> type_of_data;
        if (type_of_data == 'k') {
            in_strm >> data;
            pine.emplace(data);
        }
        else if (type_of_data == 'q') {
            in_strm >> l_bound >> u_bound;
            std::cout << pine.range_query(l_bound, u_bound) << ' ';
        }
        type_of_data = '\0';
    }
    std::cout << std::endl;
}

void run_set_and_tree(std::istream & in_strm) {
    avl::tree_t<int> pine;
    std::set<int> enemy_set;
    char type_of_data = '\0';
    int data    = 0;
    int l_bound = 0;
    int u_bound = 0;

    while(!in_strm.eof()) {
        in_strm >> type_of_data;
        if (type_of_data == 'k') {
            in_strm >> data;
            pine.insert(data);
            enemy_set.insert(data);
        }
        else if (type_of_data == 'q') {
            in_strm >> l_bound >> u_bound;

            auto tree_start_time = chrono_cur_time ();
            std::clog << "AVL tree res: ";
            std::cout << pine.range_query(l_bound, u_bound) << "\n";
            auto tree_end_time = chrono_cur_time ();
            std::clog << "AVL tree run time: " <<
                                    (tree_end_time - tree_start_time) / 0.1ms  << '\n';

            auto set_start_time = chrono_cur_time ();
            std::clog << "Set res:";
            std::cout << range_query(enemy_set, l_bound, u_bound);
            auto set_end_time = chrono_cur_time ();
            std::clog << "Set Run time: " <<
                                     (set_end_time - set_start_time) / 0.1ms  << '\n';
        }
        type_of_data = '\0';
    }
}

}
