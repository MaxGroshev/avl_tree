#pragma once
#include "avl_node.hpp"
#include "avl_range.tpp"
#include "avl_walk.tpp"

//-----------------------------------------------------------------------------------------

namespace avl {

template<typename T, typename key_type = int>
class tree_t final {
    private:
        node_t<T, key_type>* root_ = nullptr;
    public:
        tree_t(){};
        tree_t(key_type key, T data) {
            root_ = new node_t(key, data);
            assert(root_ != nullptr);
        };
        tree_t(const tree_t<T, key_type>& tree) {
            root_ = new node_t<T> (*(tree.root_));
            assert(root_ != nullptr);
            if (root_->left_ != nullptr)
                root_->left_->parent_ = root_;
            if (root_->right_ != nullptr)
                root_->right_->parent_ = root_;

        };
        tree_t(tree_t<T>&& tree) noexcept {
            root_ = tree.root_;
            tree.root_ = nullptr;
            assert(root_ != nullptr);
        };
        tree_t<T, key_type>& operator= (const tree_t<T, key_type>& tree);
        tree_t<T, key_type>& operator= (tree_t<T, key_type>&& tree);
        ~tree_t();

        inline void   insert(key_type key, T data);
        size_t range_query(int l_bound, int u_bound) const;
        size_t distance(node_t<T, key_type>* l_node, node_t<T, key_type>* u_node) const;
        inline node_t<T, key_type>* upper_bound(key_type key) const;
        inline node_t<T, key_type>* lower_bound(key_type key) const;
        inline void inorder_walk() const;
        inline void store_inorder_walk(std::vector<T>* storage) const;
        inline void graphviz_dump() const;
};

//-----------------------------------------------------------------------------------------

template<typename T, typename key_type>
tree_t<T, key_type>::~tree_t<T, key_type> () {

    if (root_ == nullptr) return;
    graphviz_dump();
    std::stack<node_t<T, key_type>*> nodes;
    nodes.push(root_);
    node_t<T, key_type>* front = nullptr;

    while(!nodes.empty()) {
        front = nodes.top();
        nodes.pop();
        if (front != nullptr) { //case of deleteing after move constr
            if (front->left_ != nullptr) {
                nodes.push(front->left_);
            }
            if (front->right_ != nullptr) {
                nodes.push(front->right_);
            }
        }
        delete front;
    }
}

template<typename T, typename key_type>
tree_t<T, key_type>& tree_t<T, key_type>::operator= (const tree_t<T, key_type>& tree) {
    if (this == &tree)
        return *this;

    node_t<T, key_type>* tmp_root_ = new node_t<T, key_type> (*(tree.root_));
    assert(tmp_root_ == nullptr);
    delete root_;
    root_ = tmp_root_;
    return *this;
}

template<typename T, typename key_type>
tree_t<T, key_type>&tree_t<T, key_type>::operator= (tree_t<T, key_type>&& tree) {
    if (this == &tree)
        return *this;

    node_t<T, key_type>* tmp_root_ = tree.root_;
    tree.root_ = nullptr;
    delete root_;
    root_ = tmp_root_;
    return *this;
}

//-----------------------------------------------------------------------------------------

template<typename T, typename key_type>
void tree_t<T, key_type>::insert(key_type key, T data) {
    if (root_ == nullptr) {
        node_t<T, key_type>* tmp_root_ = new node_t<T, key_type> (key, data);
        assert(tmp_root_ != nullptr);
        root_ = tmp_root_;
    }
    root_ = root_->insert(root_, key, data);
}

//-----------------------------------------------------------------------------------------

template<typename T, typename key_type>
node_t<T, key_type>* tree_t<T, key_type>::upper_bound(key_type key) const {
    node_t<T, key_type>* node = root_->upper_bound(key);
    assert(node != nullptr);
    return node;
}

template<typename T, typename key_type>
node_t<T, key_type>* tree_t<T, key_type>::lower_bound(key_type key) const {
    node_t<T, key_type>* node = root_->lower_bound(key);
    assert(node != nullptr);
    return node;
}

template<typename T, typename key_type>
size_t tree_t<T, key_type>::range_query(int l_bound, int u_bound) const {

    if (l_bound >= u_bound || root_ == nullptr) {
        return 0;
    }
    node_t<T, key_type>* l_node = upper_bound(u_bound);
    node_t<T, key_type>* u_node = lower_bound(l_bound);
    assert(l_node != nullptr && u_node != nullptr);

    if (u_node->key_ > u_bound || l_node->key_ < l_bound) { //corner_case
        return 0;
    }
    return distance(l_node, u_node);
}

template<typename T, typename key_type>
size_t tree_t<T, key_type>::distance(node_t<T, key_type>* l_node,
                                     node_t<T, key_type>* u_node) const {
    assert(l_node != nullptr && u_node != nullptr);
    // std::cout << l_node->key_ << '\n';
    size_t u_bound_rank = l_node->define_node_rank(root_);
    size_t l_bound_rank = u_node->define_node_rank(root_);
    // size_t res = u_bound_rank - l_bound_rank;
    return u_bound_rank - l_bound_rank + 1;
}

//-----------------------------------------------------------------------------------------

template<typename T, typename key_type>
void tree_t<T, key_type>::inorder_walk() const {
    if (root_ == nullptr) return;
    root_->inorder_walk();
    std::cout << "\n";
}

template<typename T, typename key_type>
void tree_t<T, key_type>::store_inorder_walk(std::vector<T>* storage) const {
    if (root_ == nullptr) return;
    root_->store_inorder_walk(storage);
    std::cout << "\n";
}

template<typename T, typename key_type>
void tree_t<T, key_type>::graphviz_dump() const {
    graphviz::dump_graph_t tree_dump("../graph_lib/tree_dump.dot"); //make boost::program_options

    root_->graphviz_dump(tree_dump);
    tree_dump.close_input();
}
}
