#pragma once

using namespace avl;

//-----------------------------------------------------------------------------------------

class range : public ::testing::Test {
    protected:
    avl::tree_t<int, int> tree;
    std::vector<int> correct_tree = {-14, 0, 3, 5, 11, 20, 21, 28, 42, 60};
    void SetUp() {
        std::array<int, 20> data = {5, 20, 21, -14, 0, 3, 42, 11, 45, -100,
                                    400, 68, 88, 60, 4, 5, 6, 7, 8, 28};
        for (const auto& key : data) {
            tree.insert(key, key);
        }
    }
};

//-----------------------------------------------------------------------------------------

TEST_F(range, upper_bound) {
    node_t<int, int>* node = tree.upper_bound(34);
    ASSERT_TRUE(node->key_ == 28);

    node = tree.upper_bound(800);
    ASSERT_TRUE(node->key_ == 400);
}

TEST_F(range, lower_bound) {
    node_t<int, int>* node = tree.lower_bound(-1000000);
    ASSERT_TRUE(node->key_ == -100);

    node = tree.lower_bound(-15);
    ASSERT_TRUE(node->key_ == -14);
}

