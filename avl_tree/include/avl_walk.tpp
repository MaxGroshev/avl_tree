#pragma once
#include "./avl_node.hpp"

namespace avl {

template<typename T, typename key_type>
void node_t<T, key_type>::inorder_walk() const {
    std::cout << " ( ";
    if (left_ != nullptr) {
        left_->inorder_walk();
    }
    std::cout << key_;
    if (right_ != nullptr) {
        right_->inorder_walk();
    }
    std::cout << " ) ";
}

template<typename T, typename key_type>
void node_t<T, key_type>::store_inorder_walk(std::vector<T>* storage) const {
    if (left_ != nullptr) {
        left_->store_inorder_walk(storage);
    }
    storage->push_back(key_);
    if (right_ != nullptr) {
        right_->store_inorder_walk(storage);
    }
}

template<typename T, typename key_type>
void node_t<T, key_type>::graphviz_dump(graphviz::dump_graph_t& tree_dump) const {
    tree_dump.graph_node.print_node(this, tree_dump.graphviz_strm);

    if (left_ != nullptr)
    {
        tree_dump.graph_edge.fillcolor = "#7FC7FF";
        tree_dump.graph_edge.color     = "#7FC7FF";
        tree_dump.graph_edge.print_edge(this, left_, tree_dump.graphviz_strm);
        left_->graphviz_dump(tree_dump);
    }
    if (right_ != nullptr)
    {
        tree_dump.graph_edge.fillcolor = "#DC143C";
        tree_dump.graph_edge.color     = "#DC143C";
        tree_dump.graph_edge.print_edge(this, right_, tree_dump.graphviz_strm);
        right_->graphviz_dump(tree_dump);
    }
}
}
