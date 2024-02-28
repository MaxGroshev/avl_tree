#pragma once
#include "avl_node.hpp"

//-----------------------------------------------------------------------------------------

namespace avl {

template< typename key_type = int>
class tree_t final {
    using unique_ptr_node_t = typename std::unique_ptr<node_t<key_type>>;

    unique_ptr_node_t root_ = nullptr;
    public:
        tree_t(){};
        ~tree_t();
        tree_t(const key_type& key) {
            root_ = std::make_unique<node_t<key_type>>(key);
            assert(root_ != nullptr);
        };
        tree_t(const tree_t<key_type>& tree) {

            root_ = std::make_unique<node_t<key_type>>(*(tree.root_));
            unique_ptr_node_t& root_left  = root_->get_left();
            unique_ptr_node_t& root_right = root_->get_right();
            if (root_left != nullptr)
                root_left->set_parent(root_.get());
            if (root_right != nullptr)
                root_right->set_parent(root_.get());
        };
        tree_t(tree_t<key_type>&& tree) = default;

        tree_t<key_type>& operator= (tree_t<key_type>&& tree) = default;
        tree_t<key_type>& operator= (const tree_t<key_type>& tree);

        void   insert(const key_type& key);
        template<typename... Args>

        void   emplace(Args&&... args);
        size_t range_query(const int l_bound, const int u_bound) const;
        size_t distance(const wrap_node_t<key_type>& l_node, const wrap_node_t<key_type>& u_node) const;
        wrap_node_t<key_type> upper_bound(const key_type& key) const;
        wrap_node_t<key_type> lower_bound(const key_type& key) const;
        std::vector<key_type> store_inorder_walk() const;
        void graphviz_dump() const;
};

//-----------------------------------------------------------------------------------------

template< typename key_type>
tree_t<key_type>::~tree_t<key_type> () {

    if (root_ == nullptr) return;
    std::stack<std::unique_ptr<node_t<key_type>>> nodes;
    nodes.push(std::move(root_));
    std::unique_ptr<node_t<key_type>> front = nullptr;

    while(!nodes.empty()) {
        front = std::move(nodes.top());
        nodes.pop();
        if (front != nullptr) {
            if (front->get_left() != nullptr) {
                nodes.push(std::move(front->get_left()));
            }
            if (front->get_right() != nullptr) {
                nodes.push(std::move(front->get_right()));
            }
        }
        front.reset();
    }
}

//-----------------------------------------------------------------------------------------

template< typename key_type>
tree_t<key_type>& tree_t<key_type>::operator= (const tree_t<key_type>& tree) {
    if (this == &tree)
        return *this;

    std::unique_ptr<node_t<key_type>> tmp_root_ =
                                    std::make_unique<node_t<key_type>>(*(tree.root_));
    assert(tmp_root_ == nullptr);
    root_ = std::move(tmp_root_);

    return *this;
}

//-----------------------------------------------------------------------------------------

template< typename key_type>
void tree_t<key_type>::insert(const key_type& key) {
    if (root_ == nullptr) {
        std::unique_ptr<node_t<key_type>> tmp_root_ =
                                        std::make_unique<node_t<key_type>>(key);
        assert(tmp_root_ != nullptr);
        root_ = std::move(tmp_root_);
    }
    // std::cout << "Here\n" << key << std::endl;
    root_ = root_->insert(root_, key);
    // std::cout << "out   \n" << std::endl;

    root_->set_parent(nullptr);
}

template< typename key_type>
template<typename... Args>
void tree_t<key_type>::emplace(Args&&... args) {

    key_type key = {std::move(args)...};
    if (root_ == nullptr) {
        std::unique_ptr<node_t<key_type>> tmp_root_ =
                         std::make_unique<node_t<key_type>>(std::forward<key_type>(key));
        assert(tmp_root_ != nullptr);
        root_ = std::move(tmp_root_);
    }
    root_ = root_->emplace(root_, std::forward<key_type>(key));

    root_->set_parent(nullptr);
}

//-----------------------------------------------------------------------------------------

template< typename key_type>
wrap_node_t<key_type> tree_t<key_type>::upper_bound(const key_type& key) const {
    node_t<key_type>* node = root_->upper_bound(root_.get(), key);
    assert(node != nullptr);
    return wrap_node_t{node};
}

template< typename key_type>
wrap_node_t<key_type>  tree_t<key_type>::lower_bound(const key_type& key) const {
    node_t<key_type>*  node = root_->lower_bound(root_.get(), key);
    assert(node != nullptr);
    return wrap_node_t{node};
}

template< typename key_type>
size_t tree_t<key_type>::range_query(const int l_bound, const int u_bound) const {

    if (l_bound >= u_bound || root_ == nullptr) {
        return 0;
    }
    auto l_node = upper_bound(u_bound);
    auto u_node = lower_bound(l_bound);
    // assert(l_node != nullptr && u_node != nullptr);

    if (u_node.get_key() > u_bound || l_node.get_key() < l_bound) { //corner_case
        return 0;
    }
    return distance(l_node, u_node);
}

template< typename key_type>
size_t tree_t<key_type>::distance(const wrap_node_t<key_type>& l_node,
                                    const wrap_node_t<key_type>& u_node) const {
    assert(l_node != nullptr && u_node != nullptr);
    size_t u_bound_rank = l_node.define_node_rank(root_.get());
    size_t l_bound_rank = u_node.define_node_rank(root_.get());
    return u_bound_rank - l_bound_rank + 1;
}

//-----------------------------------------------------------------------------------------

template< typename key_type>
std::vector<key_type> tree_t<key_type>::store_inorder_walk() const {
    if (root_ == nullptr) {
        return std::vector<key_type> {};
    }
    return root_->store_inorder_walk();
}

template< typename key_type>
void tree_t<key_type>::graphviz_dump() const {
    graphviz::dump_graph_t tree_dump("../graph_lib/tree_dump.dot"); //make boost::program_options

    root_->graphviz_dump(tree_dump);
    tree_dump.close_input();
}
}
