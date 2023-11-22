#include "BinarySearchTree.hpp"
#include "unit_test_framework.hpp"

TEST(basic_ctor) {
    BinarySearchTree<int> tree;
    ASSERT_TRUE(tree.empty());
    ASSERT_EQUAL(tree.size(), 0);
    ASSERT_EQUAL(tree.height(), 0);
}

TEST(basic_insert) {
    BinarySearchTree<int> tree;
    tree.insert(10);
    ASSERT_FALSE(tree.empty());
    ASSERT_EQUAL(tree.size(), 1);
    ASSERT_EQUAL(tree.height(), 1);

    tree.insert(15);
    ASSERT_EQUAL(tree.size(), 2);
    ASSERT_EQUAL(tree.height(), 2);

    tree.insert(3);
    ASSERT_EQUAL(tree.size(), 3);
    ASSERT_EQUAL(tree.height(), 2);
}

TEST(height) {
    BinarySearchTree<int> tree;
    tree.insert(10);
    tree.insert(15);
    tree.insert(3);

    ASSERT_EQUAL(tree.height(), 2);

    tree.insert(1);
    ASSERT_EQUAL(tree.height(), 3);

    tree.insert(50);
    ASSERT_EQUAL(tree.height(), 3);
    ASSERT_EQUAL(tree.size(), 5);

    tree.insert(100);
    ASSERT_EQUAL(tree.height(),4);

    tree.insert(40);
    ASSERT_EQUAL(tree.height(), 4);
    ASSERT_EQUAL(tree.size(), 7);
}

TEST(min_max_basic) {
    BinarySearchTree<int> tree;
    tree.insert(15);
    tree.insert(20);
    tree.insert(35);
    tree.insert(30);
    tree.insert(5);
    tree.insert(7);
    tree.insert(3);

    ASSERT_EQUAL(*tree.min_element(), 3);
    ASSERT_EQUAL(*tree.max_element(), 35);
    ASSERT_EQUAL(tree.size(), 7);
}

TEST(min_max_single) {
    BinarySearchTree<int> tree;
    tree.insert(10);
    ASSERT_EQUAL(*tree.min_element(), 10);
    ASSERT_EQUAL(*tree.max_element(), 10);
}

TEST(find) {
    BinarySearchTree<int> tree;
    tree.insert(15);
    tree.insert(20);
    tree.insert(17);
    tree.insert(21);
    tree.insert(3);
    tree.insert(10);

    ASSERT_EQUAL(*tree.find(17), 17);
    ASSERT_EQUAL(*tree.find(15), 15);
    ASSERT_EQUAL(*tree.find(3), 3);
}


TEST(find_single) {
    BinarySearchTree<int> tree;
    tree.insert(10);

    ASSERT_EQUAL(tree.find(10), tree.begin());
}

TEST(sorting_invariant) {
    BinarySearchTree<int> tree;
    tree.insert(10);
    tree.insert(15);
    tree.insert(3);
    tree.insert(20);

    ASSERT_TRUE(tree.check_sorting_invariant());
}

TEST(sorting_invariant_single) {
    BinarySearchTree<int> tree;
    tree.insert(5);

    ASSERT_TRUE(tree.check_sorting_invariant());
}

TEST_MAIN()
