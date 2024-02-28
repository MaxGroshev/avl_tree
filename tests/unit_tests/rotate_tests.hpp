#pragma once

using namespace avl;

//-----------------------------------------------------------------------------------------

class rotates : public ::testing::Test {
    protected:
    avl::tree_t<int> tree;
    std::vector<int> correct_tree_left_case  = {-300, -200, -100, 0, 200, 500};
    std::vector<int> correct_tree_right_case = {-100, 200, 350, 400, 500, 600};
    std::vector<int> correct_tree_lr_case = {-200, -100, -75, -50, 200, 500};
    std::vector<int> correct_tree_rl_case = {-100, 200, 300, 350, 500, 600};
    void SetUp() {
        std::array<int, 3> data = {-100, 200, 500};
        for (const auto& key : data) {
            tree.insert(key);
        }
    }
};

//-----------------------------------------------------------------------------------------

TEST_F(rotates, insert_with_left_rotate) {


    tree.insert(-200);
    tree.insert(0);
    tree.insert(-300);
    std::vector<int> storage = tree.store_inorder_walk();

    // for (const auto& elem : storage) {
    //     std::cout << elem << ' ';
    // }

    ASSERT_TRUE(storage == correct_tree_left_case);
}

TEST_F(rotates, insert_with_right_rotate) {
    tree.insert(600);
    tree.insert(400);
    tree.insert(350);

    std::vector<int> storage = tree.store_inorder_walk();
    ASSERT_TRUE(storage == correct_tree_right_case);
}

TEST_F(rotates, insert_with_left_right_rotate) {
    tree.insert(-200);
    tree.insert(-50);
    tree.insert(-75);

    std::vector<int> storage = tree.store_inorder_walk();
    ASSERT_TRUE(storage == correct_tree_lr_case);
}

TEST_F(rotates, insert_with_right_left_rotate) {
    tree.insert(300);
    tree.insert(600);
    tree.insert(350);

    std::vector<int> storage = tree.store_inorder_walk();
    ASSERT_TRUE(storage == correct_tree_rl_case);
}
