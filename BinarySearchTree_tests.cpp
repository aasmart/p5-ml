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

    ASSERT_EQUAL(tree.size(), 1);
    ASSERT_FALSE(tree.empty());

    ASSERT_EQUAL(*tree.min_element(), 10);
    ASSERT_EQUAL(*tree.max_element(), 10);
}

TEST(min_max_empty) {
    BinarySearchTree<int> tree;

    ASSERT_TRUE(tree.empty());

    ASSERT_EQUAL(tree.min_element(), tree.end());
    ASSERT_EQUAL(tree.max_element(), tree.end());
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
    ASSERT_EQUAL(tree.find(100), tree.end());
}

TEST(find_empty) {
    BinarySearchTree<int> tree;
    ASSERT_EQUAL(tree.find(3), tree.end());
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

TEST(sorting_invariant_invalid) {
    BinarySearchTree<int> tree;
    tree.insert(5);
    tree.insert(10);
    tree.insert(3);
    tree.insert(6);

    ASSERT_TRUE(tree.check_sorting_invariant());

    *tree.find(3) = 25;

    ASSERT_FALSE(tree.check_sorting_invariant());
}

TEST(sorting_invariant_invalid_duplicate) {
    BinarySearchTree<int> tree;
    tree.insert(5);
    tree.insert(10);
    tree.insert(3);
    tree.insert(6);

    ASSERT_TRUE(tree.check_sorting_invariant());

    *tree.find(3) = 5;

    ASSERT_FALSE(tree.check_sorting_invariant());
}

TEST(sorting_invariant_single) {
    BinarySearchTree<int> tree;
    tree.insert(5);

    ASSERT_TRUE(tree.check_sorting_invariant());
}

TEST(sorting_invariant_empty) {
    BinarySearchTree<int> tree;
    ASSERT_TRUE(tree.check_sorting_invariant());
}

TEST(traversal) {
    BinarySearchTree<int> tree;
    tree.insert(10);
    tree.insert(5);
    tree.insert(12);
    tree.insert(7);

    std::stringstream actual;
    tree.traverse_preorder(actual);
    ASSERT_EQUAL(tree.size(), 4);
    ASSERT_EQUAL(actual.str(), "10 5 7 12 ");

    std::stringstream inorder;
    tree.traverse_inorder(inorder);
    ASSERT_EQUAL(inorder.str(), "5 7 10 12 ")
}

TEST(traversal_single) {
    BinarySearchTree<int> tree;
    tree.insert(5);

    std::stringstream preorder;
    tree.traverse_preorder(preorder);
    ASSERT_EQUAL(tree.size(), 1);
    ASSERT_EQUAL(preorder.str(), "5 ");

    std::stringstream inorder;
    tree.traverse_inorder(inorder);
    ASSERT_EQUAL(inorder.str(), "5 ")
}

TEST(traversal_empty) {
    BinarySearchTree<int> tree;

    std::stringstream preorder;
    tree.traverse_preorder(preorder);
    ASSERT_EQUAL(tree.size(), 0);
    ASSERT_EQUAL(preorder.str(), "");

    std::stringstream inorder;
    tree.traverse_inorder(inorder);
    ASSERT_EQUAL(inorder.str(), "")
}

TEST(min_greater_than_basic) {
    BinarySearchTree<int> tree;
    tree.insert(10);
    tree.insert(3);
    tree.insert(6);
    tree.insert(15);
    tree.insert(12);
    tree.insert(16);

    ASSERT_EQUAL(tree.size(), 6);
    ASSERT_EQUAL(tree.height(), 3);

    ASSERT_EQUAL(*tree.min_greater_than(2), 3);
    ASSERT_EQUAL(*tree.min_greater_than(10), 12);
    ASSERT_EQUAL(*tree.min_greater_than(14), 15);
}

TEST(min_greater_than_contained) {
    BinarySearchTree<int> tree;
    tree.insert(10);
    tree.insert(3);
    tree.insert(6);
    tree.insert(15);
    tree.insert(12);
    tree.insert(16);

    ASSERT_EQUAL(*tree.min_greater_than(3), 6);

    tree.insert(5);

    ASSERT_EQUAL(*tree.min_greater_than(3), 5);
    ASSERT_EQUAL(*tree.min_greater_than(12), 15);
}

TEST(min_greater_than_none) {
    BinarySearchTree<int> tree;
    tree.insert(10);
    tree.insert(3);
    tree.insert(6);
    tree.insert(15);
    tree.insert(12);
    tree.insert(16);

    ASSERT_EQUAL(tree.min_greater_than(30), tree.end());
}

TEST(min_greater_than_single) {
    BinarySearchTree<int> tree;
    tree.insert(4);

    ASSERT_EQUAL(tree.min_greater_than(3), tree.begin());
    ASSERT_EQUAL(tree.min_greater_than(5), tree.end());
}

TEST(min_greater_than_empty) {
    BinarySearchTree<int> tree;
    ASSERT_EQUAL(tree.size(), 0);
    ASSERT_TRUE(tree.empty());
    ASSERT_EQUAL(tree.min_greater_than(5), tree.end());
}

TEST(copy) {
    BinarySearchTree<int> tree;
    tree.insert(6);
    tree.insert(3);
    tree.insert(10);
    tree.insert(12);

    BinarySearchTree<int> tree_2 = tree;
    ASSERT_EQUAL(*tree.begin(), *tree_2.begin());
    ASSERT_NOT_EQUAL(tree.begin(), tree_2.begin())
    ASSERT_EQUAL(tree.size(), tree_2.size());

    auto it = tree.begin();
    it++;
    ASSERT_EQUAL(*it, 6);
    it++;
    *it = 11;
    ASSERT_EQUAL(*it, 11);

    auto it2 = tree_2.begin();
    ++++it2;
    ASSERT_EQUAL(*it2, 10);
}

TEST(copy_ctor) {
    BinarySearchTree<int> tree;
    tree.insert(6);
    tree.insert(3);
    tree.insert(10);
    tree.insert(12);

    BinarySearchTree<int> tree_2(tree);
    ASSERT_EQUAL(*tree.begin(), *tree_2.begin());
    ASSERT_NOT_EQUAL(tree.begin(), tree_2.begin())
    ASSERT_EQUAL(tree.size(), tree_2.size());

    auto it = tree.begin();
    it++;
    ASSERT_EQUAL(*it, 6);
    it++;
    *it = 11;
    ASSERT_EQUAL(*it, 11);

    auto it2 = tree_2.begin();
    ++++it2;
    ASSERT_EQUAL(*it2, 10);
}

TEST(delete_copy) {
    BinarySearchTree<int> tree;
    tree.insert(7);
    tree.insert(3);

    BinarySearchTree<int> tree2;
    tree2.insert(10);

    tree2 = tree;
    ASSERT_NOT_EQUAL(tree.begin(), tree2.begin());
    ASSERT_EQUAL(*++tree2.begin(), 7);
    ASSERT_EQUAL(tree.size(), tree2.size());
}

TEST(enumerate) {
    BinarySearchTree<int> tree;
    tree.insert(15);
    tree.insert(12);
    tree.insert(20);
    tree.insert(6);
    tree.insert(13);
    tree.insert(19);

    std::stringstream result;

    for(const auto e : tree) {
        result << e << " ";
    }

    ASSERT_EQUAL(result.str(), "6 12 13 15 19 20 ");
}

TEST_MAIN()
