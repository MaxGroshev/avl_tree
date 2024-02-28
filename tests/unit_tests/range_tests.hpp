#pragma once

using namespace avl;

//-----------------------------------------------------------------------------------------

class range : public ::testing::Test {
    protected:
    avl::tree_t<int> tree;
    std::vector<int> correct_tree = {-14, 0, 3, 5, 11, 20, 21, 28, 42, 60};
    void SetUp() {
        std::array<int, 20> data = {5, 20, 21, -14, 0, 3, 42, 11, 45, -100,
                                    400, 68, 88, 60, 4, 5, 6, 7, 8, 28};
        for (const auto& key : data) {
            tree.insert(key);
        }
    }
};

//----------------c-------------------------------------------------------------------------

TEST_F(range, upper_bound) {
    auto node = tree.upper_bound(34);
    ASSERT_TRUE(node.get_key() == 28);

    node = tree.upper_bound(800);
    ASSERT_TRUE(node.get_key() == 400);
}

TEST_F(range, lower_bound) {
    auto node = tree.lower_bound(-1000000);
    ASSERT_TRUE(node.get_key() == -100);

    node = tree.lower_bound(-15);
    ASSERT_TRUE(node.get_key() == -14);
}

