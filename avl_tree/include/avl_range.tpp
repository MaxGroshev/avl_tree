#pragma once
#include "./avl_node.hpp"

namespace avl {

//-----------------------------------------------------------------------------------------

template<typename T, typename key_type>
node_t<T, key_type>* node_t<T, key_type>::upper_bound(key_type key) {

    node_t<T, key_type>* node = nullptr;
    if (key_ < key) {
        if (right_ != nullptr)
            node = right_->upper_bound(key);
        else
            return this;
    }
    else if (key_ > key) {
        if (left_ != nullptr)
            node = left_->upper_bound(key);
        else
            return this;
    }
    else if (key_ == key) {
        return this;
    }

    if (node->key_ > key && key_ < key) {
        return this;
    }
    return node;
}

template<typename T, typename key_type>
node_t<T, key_type>* node_t<T, key_type>::lower_bound(key_type key) {

    node_t<T, key_type>* node = nullptr;
    if (key_ < key) {
        if (right_ != nullptr)
            node = right_->lower_bound(key);
        else
            return this;
    }
    else if (key_ > key) {
        if (left_ != nullptr)
            node = left_->lower_bound(key);
        else
            return this;
    }
    else if (key_ == key) {
        return this;
    }

    if (node->key_ < key && key_ > key) {
        return this;
    }
    return node;
}

//-----------------------------------------------------------------------------------------

template<typename T, typename key_type>
size_t node_t<T, key_type>::define_node_rank(node_t<T, key_type>* root) const {

    size_t rank = 1;
    if (left_ != nullptr) {
        rank += left_->size_;
    }
    const node_t<T, key_type>* cur_node = this;
    while (cur_node != root) {
        if (cur_node == cur_node->parent_->right_) {
            rank += get_size (cur_node->parent_->left_) + 1;
        }
        cur_node = cur_node->parent_;
        // std::cout << "rank: " << rank << "\n";
    }
    return rank;
}

};
