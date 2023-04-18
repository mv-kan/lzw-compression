#include <gtest/gtest.h>
#include "../src/klzw.h"

TEST(helloworldtest, DemoGTest) {
    klzw::byte a = 'a';
    std::cout << a << std::endl;
    EXPECT_TRUE(true);
    EXPECT_EQ(a, 'a');
}

int main(int argc, char **argv)
{
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}