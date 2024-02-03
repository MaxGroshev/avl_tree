#pragma once

#include "utils.hpp"
#include <memory>
#include <stack>
#include <cassert>

//-----------------------------------------------------------------------------------------

namespace avl {

template <typename T, typename key_type = int>
class node_t {
    private:
        key_type key_;
    public:
        T data_;
        std::unique_ptr<node_t<T, key_type>> left_  = nullptr;
        std::unique_ptr<node_t<T, key_type>> right_ = nullptr;
        node_t<T, key_type>* parent_ = nullptr;
        size_t size_   = 1;
        size_t height_ = 1;


        node_t(key_type key, T data) : key_(key), data_(data) {};
        node_t(const node_t<T, key_type>& node) : key_(node.key_), data_(node.data_),
                                                  height_(node.height_) {

            std::unique_ptr<node_t<T, key_type>> ret_node = safe_copy(&node);
            left_  = std::move(ret_node->left_);
            right_ = std::move(ret_node->right_);
        }
        node_t(key_type key, T data, size_t size, size_t height) :
            key_(key), data_(data),
            size_(size), height_(height)
            {};
        std::unique_ptr<node_t<T, key_type>> safe_copy (const node_t<T, key_type>* node);
        node_t<T, key_type>& operator= (const node_t<T, key_type>& node);
        node_t(node_t<T, key_type>&& node) = default;
        node_t<T, key_type>& operator= (node_t<T, key_type>&& node) = default;


        int find_balance_fact(node_t<T, key_type>* node) const {
            if (node)
                return (get_height(node->right_.get()) - get_height(node->left_.get()));
            else
                return 0;
        }
        size_t get_height(node_t<T, key_type>* node) const {
            if (node) return node->height_; else return 0;
        }
        size_t get_size(const node_t<T, key_type>* node) const {
            if (node) return node->size_; else return 0;
        }
        key_type get_key() const {
            return key_;
        }
        void change_height(node_t<T, key_type>* node) {
            if (node) {
                node->height_ = 1 + std::max(get_height(node->left_.get()),
                                             get_height(node->right_.get()));
            }
            else
                node->height_ = 1;
        }
        void change_size(node_t<T, key_type>* node) {
            if (node) {
                node->size_ = 1 + get_size(node->left_.get()) +
                                  get_size(node->right_.get());
            }
        }


        std::unique_ptr<node_t<T, key_type>> balance_subtree(node_t<T, key_type>* cur_node, T key);
        std::unique_ptr<node_t<T, key_type>> rotate_to_left(node_t<T, key_type>* cur_node);
        std::unique_ptr<node_t<T, key_type>> rotate_to_right(node_t<T, key_type>* cur_node);
        std::unique_ptr<node_t<T, key_type>> insert(avl::node_t<T, key_type>* cur_node,
                                           T data, key_type key);


        std::vector<T> store_inorder_walk() const;
        void graphviz_dump(graphviz::dump_graph_t& tree_dump) const ;
        node_t<T, key_type>* upper_bound(avl::node_t<T, key_type>* node, key_type key);
        node_t<T, key_type>* lower_bound(avl::node_t<T, key_type>* node, key_type key);

        size_t define_node_rank(node_t<T, key_type>* root, node_t<T, key_type>* cur_node) const;
};
}

//-----------------------------------------------------------------------------------------

namespace avl {

template<typename T, typename key_type>
node_t<T, key_type>& node_t<T, key_type>::operator= (const node_t<T, key_type>& node) {
    if (this == &node)
        return *this;

    std::unique_ptr<node_t<T, key_type>> tmp_left_  =
                                std::make_unique<node_t<T, key_type>> (*(node.left_));
    std::unique_ptr<node_t<T, key_type>> tmp_right_ =
                                std::make_unique<node_t<T, key_type>> (*(node.right_));
    assert(tmp_left_ != nullptr && tmp_right_ != nullptr);

    key_   = node.key_;
    height_= node.height_;
    data_  = node.data_;

    left_  = std::move(tmp_left_);
    right_ = std::move(tmp_right_);

    return *this;
}

template<typename T, typename key_type>
std::unique_ptr<node_t<T, key_type>>
node_t<T, key_type>::safe_copy(const node_t<T, key_type>* origine_node) {

    std::cout << "Safe_copy";
    std::unique_ptr<node_t<T, key_type>> new_node =
                        std::make_unique<node_t<T, key_type>>(origine_node->key_,
                        origine_node->data_, origine_node->size_,origine_node->height_);

    node_t<T, key_type>* iter_node = new_node.get();
    while (origine_node != nullptr) {
        if (iter_node->left_ == nullptr && origine_node->left_ != nullptr) {
            // std::cout << "left" <<std::endl;
            iter_node->left_ = std::make_unique<node_t<T, key_type>>(
                            origine_node->left_->key_, origine_node->left_->data_,
                            origine_node->left_->size_, origine_node->left_->height_);
            iter_node->left_->parent_ = iter_node;

            iter_node    = iter_node->left_.get();
            origine_node = origine_node->left_.get();
        }
        else if (iter_node->right_ == nullptr && origine_node->right_ != nullptr) {
            // std::cout << "right"<<std::endl;
            iter_node->right_ = std::make_unique<node_t<T, key_type>>(
                            origine_node->right_->key_, origine_node->right_ ->data_,
                            origine_node->right_->size_, origine_node->right_->height_);
            iter_node->right_->parent_ = iter_node;

            iter_node    = iter_node->right_.get();
            origine_node = origine_node->right_.get();
        }
        else {
            iter_node = iter_node->parent_;
            origine_node = origine_node->parent_;
        }
    }
    return new_node;
}

//-----------------------------------------------------------------------------------------

template<typename T, typename key_type>
std::unique_ptr<node_t<T, key_type>> node_t<T, key_type>::insert(
                                avl::node_t<T, key_type>* cur_node, T data,key_type key) {
    if(!cur_node)
        throw("Invalid ptr");

    if (cur_node->key_ < key) {
        if (cur_node->right_ != nullptr) {
            cur_node->right_ = insert(cur_node->right_.release(), key, data);
        }
        else {
            cur_node->right_ = std::make_unique<node_t<T, key_type>>(key, data);
            assert(cur_node->right_ != nullptr);
        }
        cur_node->right_->parent_ = cur_node;
    }
    else if (cur_node->key_ > key) {
        if (cur_node->left_ != nullptr) {
            cur_node->left_ = insert(cur_node->left_.release(), key, data);
        }
        else {
            cur_node->left_ = std::make_unique<node_t<T, key_type>>(key, data);
            assert(cur_node->left_ != nullptr);
        }
        cur_node->left_->parent_ = cur_node;
    }

    change_height(cur_node);
    change_size(cur_node);

    return balance_subtree(cur_node, key);
}

//----------------------------ROTATES------------------------------------------------------

template<typename T, typename key_type>
std::unique_ptr<node_t<T, key_type>>
node_t<T, key_type>::balance_subtree(node_t<T, key_type>* cur_node, T key) {

    if(!cur_node)
        throw("Invalid ptr");

    int delta = find_balance_fact(cur_node);
    if (delta > 1) {
        if (key < cur_node->right_->key_) { //complicated condition
            // std::cout << "RR rotate";
            cur_node->right_ = rotate_to_right(cur_node->right_.release());
            cur_node->right_->parent_ = cur_node;
        }
        // std::cout << "Left_rotate"<< std::endl;
        return rotate_to_left(cur_node);
    }
    else if (delta < -1) {
        if (key > cur_node->left_->key_) {
            // std::cout << "LL rotate";
            cur_node->left_ = rotate_to_left(cur_node->left_.release());
            cur_node->left_->parent_ = cur_node;
        }
        // std::cout << "Right_rotate"<< std::endl;
        return rotate_to_right(cur_node);
    }
    else
        return std::unique_ptr<node_t<T, key_type>>(cur_node);
}

template<typename T, typename key_type>
std::unique_ptr<node_t<T, key_type>>
node_t<T, key_type>::rotate_to_left(node_t<T, key_type>* cur_node) {

    if(!cur_node)
        throw("Invalid ptr");

    node_t<T>* root = cur_node->right_.release();
    node_t<T>* root_left = root->left_.release();
    root->left_  = std::unique_ptr<node_t<T, key_type>>(cur_node);
    cur_node->right_ = std::unique_ptr<node_t<T, key_type>>(root_left);

    root->left_->parent_ = root;
    if (root_left) {
        root_left->parent_ = cur_node;
    }

    change_height(root->left_.get());
    change_height(root);
    root->size_ = root->left_->size_;
    change_size(root->left_.get());

    return std::unique_ptr<node_t<T, key_type>>(root);;
}

template<typename T, typename key_type>
std::unique_ptr<node_t<T, key_type>>
node_t<T, key_type>::rotate_to_right(node_t<T, key_type>* cur_node) {

    if(!cur_node)
        throw("Invalid ptr");

    node_t<T>* root = cur_node->left_.release();
    node_t<T>* root_right = root->right_.release();
    root->right_ = std::unique_ptr<node_t<T, key_type>>(cur_node);
    cur_node->left_ = std::unique_ptr<node_t<T, key_type>>(root_right);

    root->right_->parent_ = root;
    if (root_right) {
        root_right->parent_ = cur_node;
    }

    change_height(root->right_.get());
    change_height(root);
    root->size_ = root->right_->size_;
    root->right_->size_ = get_size(root->right_->right_.get()) +
                          get_size(root->right_->left_.get()) + 1;

    return std::unique_ptr<node_t<T, key_type>>(root);;
}

//--------------------RANGES---------------------------------------------------------------

template<typename T, typename key_type>
node_t<T, key_type>*
node_t<T, key_type>::upper_bound(node_t<T, key_type>* cur_node, key_type key) {

    node_t<T, key_type>* node = nullptr;
    if (cur_node->key_ < key) {
        if (cur_node->right_ != nullptr)
            node = upper_bound(cur_node->right_.get(), key);
        else
            return cur_node;
    }
    else if (cur_node->key_ > key) {
        if (cur_node->left_ != nullptr)
            node = upper_bound(cur_node->left_.get(), key);
        else
            return cur_node;
    }
    else if (cur_node->key_ == key) {
        return cur_node;
    }

    if (node->key_ > key && cur_node->key_ < key) {
        return cur_node;
    }
    return node;
}

template<typename T, typename key_type>
node_t<T, key_type>*
node_t<T, key_type>::lower_bound(node_t<T, key_type>* cur_node, key_type key) {

    node_t<T, key_type>*  node = nullptr;
    if (cur_node->key_ < key) {
        if (cur_node->right_ != nullptr)
            node = lower_bound(cur_node->right_.get(), key);
        else
            return cur_node;
    }
    else if (cur_node->key_ > key) {
        if (cur_node->left_ != nullptr)
            node = lower_bound(cur_node->left_.get(), key);
        else
            return cur_node;
    }
    else if (cur_node->key_ == key) {
        return cur_node;
    }

    if (node->key_ < key && cur_node->key_ > key) {
        return cur_node;
    }
    return node;
}

//-----------------------------------------------------------------------------------------

template<typename T, typename key_type>
size_t node_t<T, key_type>::define_node_rank(node_t<T, key_type>* root,
                                             node_t<T, key_type>* cur_node) const {

    if(cur_node == nullptr)
        throw("Invalid ptr");

    size_t rank = 1;
    if (cur_node->left_ != nullptr) {
        rank += cur_node->left_->size_;
    }
    const node_t<T, key_type>* tmp_node = this;
    while (tmp_node != root) {
        if (tmp_node == tmp_node->parent_->right_.get()) {
            rank += get_size (tmp_node->parent_->left_.get()) + 1;
        }
        tmp_node = tmp_node->parent_;
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
                cur_node = cur_node->right_.get();
            else
                cur_node = nullptr;

            node_stk.pop();
        }
        while (cur_node) {
            node_stk.push(cur_node);
            cur_node = cur_node->left_.get();
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

