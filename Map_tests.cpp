#include "Map.hpp"
#include "unit_test_framework.hpp"

TEST(map_ctor) {
    Map<std::string, int> map;
    ASSERT_EQUAL(map.begin(), map.end());
    ASSERT_EQUAL(map.size(), 0);
    ASSERT_TRUE(map.empty());
}

TEST(insert) {
    Map<std::string, int> map;
    ASSERT_TRUE(map.insert(std::pair{"test", 5}).second);

    ASSERT_FALSE(map.empty());
    ASSERT_EQUAL(map.size(), 1);

    ASSERT_TRUE(map.insert(std::pair{"hello", 7}).second);
    ASSERT_EQUAL(map.size(), 2);

    ASSERT_EQUAL((*map.begin()).second, 7);
    ASSERT_EQUAL((*++map.begin()).second, 5);

    ASSERT_TRUE(map.insert(std::pair("zed", 2)).second);
    ASSERT_EQUAL(map.size(), 3);
    ASSERT_EQUAL((*++++map.begin()).second, 2);
}

TEST(insert_contains) {
    Map<std::string, int> map;
    auto res = map.insert(std::pair{"hello", 1});
    ASSERT_TRUE(res.second);

    ASSERT_EQUAL((*res.first).first, "hello");
    ASSERT_EQUAL((*res.first).second, 1);

    res = map.insert(std::pair{"hello", 1});
    ASSERT_FALSE(res.second);
    ASSERT_EQUAL((*res.first).first, "hello");
    ASSERT_EQUAL((*res.first).second, 1);

    res = map.insert(std::pair{"hello", 3});
    ASSERT_FALSE(res.second);
    ASSERT_EQUAL((*res.first).first, "hello");
    ASSERT_EQUAL((*res.first).second, 1);

    ASSERT_EQUAL(map.size(), 1);
    ASSERT_FALSE(map.empty());
}

TEST(index_first) {
    Map<std::string, int> map;
    map["hello"] = 3;

    ASSERT_EQUAL(map.size(), 1);
    ASSERT_FALSE(map.empty());
    ASSERT_EQUAL((*map.begin()).second, 3);

    map["hello"] = 3;
    ASSERT_EQUAL(map.size(), 1);

    map["test"] = 2;
    ASSERT_EQUAL(map.size(), 2);

    map.insert(std::pair{"world", 10});
    ASSERT_EQUAL(map.size(), 3);

    ASSERT_EQUAL((*++map.begin()).second, 2);
}

TEST(insert_index) {
    Map<std::string, std::string> map;
    map.insert(std::pair{"hello", "world"});

    ASSERT_EQUAL(map.size(), 1);

    map["world"] = "hello";
    ASSERT_EQUAL(map.size(), 2);

    map["hello"] = "world";
    ASSERT_EQUAL(map.size(), 2);
}

TEST(index_edit) {
    Map<std::string, std::string> map;
    map["test"] = "123";

    ASSERT_EQUAL((*map.begin()).second, "123");

    map["test"] = "456";
    ASSERT_EQUAL((*map.begin()).second, "456");
}

TEST_MAIN()
