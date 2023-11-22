#include "Map.hpp"
#include "unit_test_framework.hpp"

TEST(map_ctor) {
    Map<std::string, int> map;
    ASSERT_EQUAL(map.begin(), map.end());
    ASSERT_EQUAL(map.size(), 0);
    ASSERT_TRUE(map.empty());
}

TEST_MAIN()
