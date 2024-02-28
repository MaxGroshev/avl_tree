#pragma once

using namespace avl;

//-----------------------------------------------------------------------------------------

class big_five : public ::testing::Test {
    protected:
    avl::tree_t<int> tree;
    std::vector<int> correct_tree = {-14, 0, 3, 5, 11, 20, 21, 28, 42, 60};
    void SetUp() {
        std::array<int, 10> data = {5, 20, 21, -14, 0, 3, 42, 11, 60, 28};
        for (const auto& key : data) {
            tree.insert(key);
        }
    }
};


//-----------------------------------------------------------------------------------------

TEST_F(big_five, constructor_test) {
    std::vector<int> storage = tree.store_inorder_walk();
    ASSERT_TRUE(storage == correct_tree);
}

TEST_F(big_five, copy_constructor_test) {
    tree_t<int> pine {tree};
    std::vector<int> tree_storage = tree.store_inorder_walk();
    std::vector<int> pine_storage = pine.store_inorder_walk();

//     for (const auto & elem : pine_storage)
//         std::cout << elem << ' ';
//
//     std::cout << "\n\n\n\n";

    ASSERT_TRUE(tree_storage == correct_tree);
    ASSERT_TRUE(pine_storage == correct_tree);
}

TEST_F(big_five, copy_assignment_test) {
    tree_t<int> pine = tree;
    std::vector<int> tree_storage = tree.store_inorder_walk();
    std::vector<int> pine_storage = pine.store_inorder_walk();

    ASSERT_TRUE(tree_storage == correct_tree);
    ASSERT_TRUE(pine_storage == correct_tree);
}

TEST_F(big_five, move_constructor_test) {
    tree_t<int> pine {std::move(tree)};
    std::vector<int> pine_storage = pine.store_inorder_walk();

    ASSERT_TRUE(pine_storage == correct_tree);
}

TEST_F(big_five, move_assignment_test) {
    tree_t<int> pine = std::move(tree);
    std::vector<int> pine_storage = pine.store_inorder_walk();

    ASSERT_TRUE(pine_storage == correct_tree);
}
