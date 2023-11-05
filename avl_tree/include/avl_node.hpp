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


        node_t(key_type key, T data) : key_(key), data_(data) {};
        node_t(const node_t<T, key_type>& node) : key_(node.key_), data_(node.data_),
                                                                   height_(node.height_) {
            if (node.left_ != nullptr) {
                left_ = new node_t<T, key_type>(*node.left_);
                ASSERT(left_ != nullptr);
            }
            if (node.right_ != nullptr) {
                right_ =  new node_t<T, key_type>(*node.right_);
                ASSERT(right_ != nullptr);
            }
        }
        node_t(node_t<T>&& node) noexcept: key_(node.key_),      data_(node.data_),
                                           parent_(node.parent), height_(node.height_),
                                           left_(node.left_),    right_(node.right_) {
            node.right_ = nullptr;
            node.left_  = nullptr;
            node.prent_ = nullptr;
        }
        node_t<T, key_type>& operator= (const node_t<T, key_type>& node);
        node_t<T, key_type>& operator= (node_t<T, key_type>&& node);
        ~node_t() {
            delete right_;
            delete left_;
        }


        inline int find_balance_fact() const {
            return (get_height(right_) - get_height(left_));
        }
        inline size_t get_height(node_t<T, key_type>* node) const {
            if (node) return node->height_; else return 0;
        }
        inline void change_height() {
            height_ = 1 + find_max(get_height(left_), get_height(right_));
        }
        inline size_t get_size(node_t<T, key_type>* node) const {
            if (node) return node->size_; else return 0;
        }


        inline node_t<T, key_type>* balance_subtree(T key);
        inline node_t<T, key_type>* rotate_to_left();
        inline node_t<T, key_type>* rotate_to_right();
        inline node_t<T, key_type>* insert(avl::node_t<T, key_type>* cur_node,
                                           T data, key_type key);


        inline void inorder_walk() const;
        inline void store_inorder_walk(std::vector<T>* storage) const;
        inline void graphviz_dump(graphviz::dump_graph_t& tree_dump) const ;
        inline node_t<T, key_type>* upper_bound(key_type key);
        inline node_t<T, key_type>* lower_bound(key_type key);

        inline size_t define_node_rank(node_t<T, key_type>* root) const;
};
}
#include "./avl_walk.tpp"
#include "./avl_range.tpp"
//-----------------------------------------------------------------------------------------

namespace avl {

template<typename T, typename key_type>
node_t<T, key_type>& node_t<T, key_type>::operator= (const node_t<T, key_type>& node) {
    if (this == &node)
        return *this;

    delete left_;
    delete right_;
    delete parent_;

    key_ = node.key_;
    height_ = node.height_;
    data_ = node.data_;
    left_ = new node_t<T> (*(node.left_));
    left_ = new node_t<T> (*(node.right_));
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
        }
        right_->parent_ = this;
    }
    else if (cur_node->key_ > key) {
        if (left_ != nullptr) {
            left_ = left_->insert(left_, key, data);
        }
        else {
            left_ = new node_t<T> (key, data);
        }
        left_->parent_ = this;
    }

    change_height();
    size_ = get_size(left_) + get_size(right_) + 1;
    return balance_subtree(key);
}

//-----------------------------------------------------------------------------------------

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
}
