#pragma once

#include "utils.hpp"
#include <stack>

//-----------------------------------------------------------------------------------------

namespace avl {

template <typename T, typename key_type = int>
class node_t {
    public:
        key_type key_;
        T data_;
        node_t<T, key_type>* left_  = nullptr;
        node_t<T, key_type>* right_ = nullptr;
        node_t<T, key_type>* parent_= nullptr;
        size_t size_   = 1;
        size_t height_ = 1;

    private:
        node_t(key_type key, T data, size_t size, size_t height) :
            key_(key), data_(data),
            size_(size), height_(height)
            {};

    public:
        node_t(key_type key, T data) : key_(key), data_(data) {};
        node_t(const node_t<T, key_type>& node) : key_(node.key_), data_(node.data_),
                                                  height_(node.height_) {

            node_t<T, key_type>* ret_node = safe_copy(&node);
            left_  = ret_node->left_;
            right_ = ret_node->right_;

            delete ret_node;
        }
        node_t(node_t<T>&& node) noexcept: key_(node.key_),      data_(node.data_),
                                           parent_(node.parent), height_(node.height_),
                                           left_(node.left_),    right_(node.right_) {
            node.right_ = nullptr;
            node.left_  = nullptr;
            node.parent_ = nullptr;
        }
        node_t<T, key_type>* safe_copy (const node_t<T, key_type>* node);
        node_t<T, key_type>& operator= (const node_t<T, key_type>& node);
        node_t<T, key_type>& operator= (node_t<T, key_type>&& node);
        ~node_t() = default;


        int find_balance_fact() const {
            return (get_height(right_) - get_height(left_));
        }
        size_t get_height(node_t<T, key_type>* node) const {
            if (node) return node->height_; else return 0;
        }
        void change_height() {
            height_ = 1 + find_max(get_height(left_), get_height(right_));
        }
        size_t get_size(node_t<T, key_type>* node) const {
            if (node) return node->size_; else return 0;
        }


        node_t<T, key_type>* balance_subtree(T key);
        node_t<T, key_type>* rotate_to_left();
        node_t<T, key_type>* rotate_to_right();
        node_t<T, key_type>* insert(avl::node_t<T, key_type>* cur_node,
                                           T data, key_type key);


        std::vector<T> store_inorder_walk() const;
        void graphviz_dump(graphviz::dump_graph_t& tree_dump) const ;
        node_t<T, key_type>* upper_bound(key_type key);
        node_t<T, key_type>* lower_bound(key_type key);

        size_t define_node_rank(node_t<T, key_type>* root) const;
};
}

//-----------------------------------------------------------------------------------------

namespace avl {

template<typename T, typename key_type>
node_t<T, key_type>& node_t<T, key_type>::operator= (const node_t<T, key_type>& node) {
    if (this == &node)
        return *this;

    node_t<T, key_type>* tmp_left_  = new node_t<T> (*(node.left_));
    node_t<T, key_type>* tmp_right_ = new node_t<T> (*(node.right_));
    assert(tmp_left_ != nullptr && tmp_right_ != nullptr);

    key_ = node.key_;
    height_ = node.height_;
    data_  = node.data_;

    delete left_;
    delete right_;
    delete parent_;

    left_ = tmp_left_;
    right_ = tmp_right_;

    return *this;
}

template<typename T, typename key_type>
node_t<T, key_type>& node_t<T, key_type>::operator= (node_t<T, key_type>&& node) {
    if (this == &node)
        return *this;

    delete left_;
    delete right_;
    delete parent_;

    left_ = node.left_;
    right_ = node.right_;
    parent_ = node.parent_;
    key_ = node.key_;
    height_ = node.height_;

    node.right_ = nullptr;
    node.left_  = nullptr;
    node.parent_ = nullptr;
    return *this;
}

template<typename T, typename key_type>
node_t<T, key_type>* node_t<T, key_type>::safe_copy(const node_t<T, key_type>* origine_node) {

    node_t<T, key_type>* new_node = new node_t<T, key_type>(origine_node->key_,
            origine_node->data_, origine_node->size_,origine_node->height_);

    node_t<T, key_type>* root = new_node;

    try {
        while (origine_node != nullptr) {
            if (new_node->left_ == nullptr && origine_node->left_ != nullptr) {
                new_node->left_ = new node_t<T, key_type>(
                                origine_node->left_->key_, origine_node->left_->data_,
                                origine_node->left_->size_, origine_node->left_->height_);
                new_node->left_->parent_ = new_node;

                new_node     = new_node->left_;
                origine_node = origine_node->left_;
            }
            else if (new_node->right_ == nullptr && origine_node->right_ != nullptr) {
                new_node->right_ = new node_t<T, key_type>(
                                origine_node->right_->key_, origine_node->right_ ->data_,
                                origine_node->right_->size_, origine_node->right_->height_);
                new_node->right_->parent_ = new_node;

                new_node = new_node->right_;
                origine_node = origine_node->right_;
            }
            else {
                new_node = new_node->parent_;
                origine_node = origine_node->parent_;
            }
        }
    } catch(...) {
        delete new_node;
        throw;
    }
    return root;
}

//-----------------------------------------------------------------------------------------

template<typename T, typename key_type>
node_t<T, key_type>* node_t<T, key_type>::insert(avl::node_t<T, key_type>* cur_node, T data,
                                                                            key_type key) {
    if (cur_node->key_ < key) {
        if (cur_node->right_ != nullptr) {
            right_ = right_->insert(right_, key, data);
        }
        else {
            right_ = new node_t<T> (key, data);
            assert(right_ != nullptr);
        }
        right_->parent_ = this;
    }
    else if (cur_node->key_ > key) {
        if (left_ != nullptr) {
            left_ = left_->insert(left_, key, data);
        }
        else {
            left_ = new node_t<T> (key, data);
            assert(left_ != nullptr);
        }
        left_->parent_ = this;
    }

    change_height();
    size_ = get_size(left_) + get_size(right_) + 1;
    return balance_subtree(key);
}

//----------------------------ROTATES------------------------------------------------------

template<typename T, typename key_type>
node_t<T, key_type>* node_t<T, key_type>::balance_subtree(T key) {

    int delta = find_balance_fact();
    if (delta > 1) {
        if (key < right_->key_) { //complicated condition
            // std::cout << "RR rotate";
            right_ = right_->rotate_to_right();
            right_->parent_ = this;
        }
        return rotate_to_left();
    }
    else if (delta < -1) {
        if (key > left_->key_) {
            // std::cout << "LL rotate";
            left_ = left_->rotate_to_left();
            left_->parent_ = this;
        }
        return rotate_to_right();
    }
    else
        return this;
}

template<typename T, typename key_type>
node_t<T, key_type>* node_t<T, key_type>::rotate_to_left() {

    node_t<T>* root = right_;
    node_t<T>* root_left = right_->left_;
    root->left_  = this;
    right_ = root_left;

    root->left_->parent_ = root;
    if (root_left) {
        root_left->parent_ = this;
    }

    root->left_->change_height();
    root->change_height();
    root->size_ = root->left_->size_;
    root->left_->size_ = get_size(root->left_->right_) +
                         get_size(root->left_->left_) + 1;

    return root;
}

template<typename T, typename key_type>
node_t<T, key_type>* node_t<T, key_type>::rotate_to_right() {

    node_t<T>* root = left_;
    node_t<T>* root_right = left_->right_;
    root->right_ = this;
    left_ = root_right;

    root->right_->parent_ = root;
    if (root_right) {
        root_right->parent_ = this;
    }

    root->right_->change_height();
    root->change_height();
    root->size_ = root->right_->size_;
    root->right_->size_ = get_size(root->right_->right_) +
                          get_size(root->right_->left_) + 1;

    return root;
}

//--------------------RANGES---------------------------------------------------------------

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

//--------------------WALKING--------------------------------------------------------------

template<typename T, typename key_type>
std::vector<T> node_t<T, key_type>::store_inorder_walk() const {
    std::vector<T> storage;
    std::stack<const node_t<T, key_type>*> node_stk;
    const node_t<T, key_type>* cur_node = this;

    while (cur_node || !node_stk.empty()) {
        if (!node_stk.empty()) {
            cur_node = node_stk.top();
            storage.push_back(cur_node->key_);
            if (cur_node->right_)
                cur_node = cur_node->right_;
            else
                cur_node = nullptr;

            node_stk.pop();
        }
        while (cur_node) {
            node_stk.push(cur_node);
            cur_node = cur_node->left_;
        }
    }

    return storage;
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
