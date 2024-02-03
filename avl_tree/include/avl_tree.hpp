#pragma once
#include "avl_node.hpp"

//-----------------------------------------------------------------------------------------

namespace avl {

template<typename T, typename key_type = int>
class tree_t final {
    private:
        std::unique_ptr<node_t<T, key_type>> root_ = nullptr;
    public:
        tree_t(){};
        tree_t(key_type key, T data) {
            root_ = std::make_unique<node_t<T, key_type>>(key, data);
            assert(root_ != nullptr);
        };
        tree_t(const tree_t<T, key_type>& tree) {

            root_ = std::make_unique<node_t<T, key_type>>(*(tree.root_));
            if (root_->left_ != nullptr)
                root_->left_->parent_ = root_.get();
            if (root_->right_ != nullptr)
                root_->right_->parent_ = root_.get();

        };
        tree_t(tree_t<T, key_type>&& tree) = default;

        tree_t<T, key_type>& operator= (tree_t<T, key_type>&& tree) = default;
        tree_t<T, key_type>& operator= (const tree_t<T, key_type>& tree);

        void   insert(key_type key, T data);
        size_t range_query(int l_bound, int u_bound) const;
        size_t distance(node_t<T, key_type>* l_node, node_t<T, key_type>* u_node) const;
        node_t<T, key_type>* upper_bound(key_type key) const;
        node_t<T, key_type>* lower_bound(key_type key) const;
        std::vector<T> store_inorder_walk() const;
        void graphviz_dump() const;
};

//-----------------------------------------------------------------------------------------

template<typename T, typename key_type>
tree_t<T, key_type>& tree_t<T, key_type>::operator= (const tree_t<T, key_type>& tree) {
    if (this == &tree)
        return *this;

    std::unique_ptr<node_t<T, key_type>> tmp_root_ =
                                     std::make_unique<node_t<T, key_type>>(*(tree.root_));
    assert(tmp_root_ == nullptr);
    root_ = std::move(tmp_root_);

    return *this;
}

//-----------------------------------------------------------------------------------------

template<typename T, typename key_type>
void tree_t<T, key_type>::insert(key_type key, T data) {
    if (root_ == nullptr) {
        std::unique_ptr<node_t<T, key_type>> tmp_root_ =
                                        std::make_unique<node_t<T, key_type>>(key, data);
        assert(tmp_root_ != nullptr);
        root_ = std::move(tmp_root_);
    }
    // std::cout << "Here\n" << key << std::endl;
    root_ = root_->insert(root_.release(), key, data);
    // std::cout << "out   \n" << std::endl;

    root_->parent_ = nullptr;
}

//-----------------------------------------------------------------------------------------

template<typename T, typename key_type>
node_t<T, key_type>* tree_t<T, key_type>::upper_bound(key_type key) const {
    node_t<T, key_type>* node = root_->upper_bound(root_.get(), key);
    assert(node != nullptr);
    return node;
}

template<typename T, typename key_type>
node_t<T, key_type>*  tree_t<T, key_type>::lower_bound(key_type key) const {
    node_t<T, key_type>*  node = root_->lower_bound(root_.get(), key);
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

    if (u_node->get_key() > u_bound || l_node->get_key() < l_bound) { //corner_case
        return 0;
    }
    return distance(l_node, u_node);
}

template<typename T, typename key_type>
size_t tree_t<T, key_type>::distance(node_t<T, key_type>* l_node,
                                     node_t<T, key_type>* u_node) const {
    assert(l_node != nullptr && u_node != nullptr);
    size_t u_bound_rank = l_node->define_node_rank(root_.get(), l_node);
    size_t l_bound_rank = u_node->define_node_rank(root_.get(), u_node);
    return u_bound_rank - l_bound_rank + 1;
}

//-----------------------------------------------------------------------------------------

template<typename T, typename key_type>
std::vector<T> tree_t<T, key_type>::store_inorder_walk() const {
    if (root_ == nullptr) {
        return std::vector<T> {};
    }
    return root_->store_inorder_walk();
}

template<typename T, typename key_type>
void tree_t<T, key_type>::graphviz_dump() const {
    graphviz::dump_graph_t tree_dump("../graph_lib/tree_dump.dot"); //make boost::program_options

    root_->graphviz_dump(tree_dump);
    tree_dump.close_input();
}
}
