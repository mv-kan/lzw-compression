#include <gtest/gtest.h>
#include <vector>
#include <fstream>
#include "../src/decompress.h"
#include <iterator>

TEST(decompress, BytesToCodesBaseCase)
{
    std::vector<klzw::code_t> testoutput{0b110001010, 0b101010101, 0b010001111, 0b1, 0b1, 0b111111101, 0b111111100, 0b101001001};
    std::vector<klzw::byte> testinput{0b10001010, 0b10101011, 0b00111110, 0b00001010, 0b00010000, 0b10100000, 0b00111111, 0b11111111, 0b10100100};
    const size_t testoffset{0};
    const size_t CODE_SIZE = 9;
    std::vector<klzw::code_t> output{};
    size_t offset{};
    size_t tmp{};
    ssize_t tmp2{CODE_SIZE};
    klzw::details::BytesToCodes(testinput, &offset, &tmp, &tmp2, output, CODE_SIZE);

    ASSERT_EQ(testoutput.size(), output.size()) << "Vectors testoutput and output are of unequal length";
    ASSERT_EQ(testoffset, offset) << "offsets are not equal";

    for (size_t i = 0; i < testoutput.size(); ++i)
    {
        EXPECT_EQ(testoutput[i], output[i]) << "Vectors testoutput and output differ at index " << i;
    }
}

TEST(decompress, BytesToCodesOffset)
{
    std::vector<klzw::code_t> testoutput{0b110001010, 0b101010101, 0b010001111, 0b1, 0b1, 0b111111101, 0b111111100, 0b101001001};
    std::vector<klzw::byte> testinput{0b00101000, 0b10101110, 0b11111010, 0b00101000, 0b01000000, 0b10000000, 0b11111110, 0b11111100, 0b10010011, 0b00000010};
    const size_t testoffset{2};
    size_t testoffsetarg{2};

    const size_t CODE_SIZE = 9;
    std::vector<klzw::code_t> output{};
    size_t tmp{};
    ssize_t tmp2{CODE_SIZE};
    klzw::details::BytesToCodes(testinput, &testoffsetarg, &tmp, &tmp2, output, CODE_SIZE);

    ASSERT_EQ(testoutput.size(), output.size()) << "Vectors testoutput and output are of unequal length";
    EXPECT_EQ(testoffset, testoffsetarg) << "offsets are not equal";

    for (size_t i = 0; i < testoutput.size(); ++i)
    {
        EXPECT_EQ(testoutput[i], output[i]) << "Vectors testoutput and output differ at index " << i;
    }
}

TEST(decompress, BytesToCodesExtendCodeSizeCase)
{
    std::vector<klzw::code_t> testoutput{0b110001010, 0b101010101, 0b010001111, 0b1, 0b1, 0b111111110, 0b1111111100, 0b1, 0b1101001001};
    std::vector<klzw::byte> testinput{0b10001010, 0b10101011, 0b00111110, 0b00001010, 0b00010000, 0b11000000, 0b00111111, 0b11111111, 0b00000001, 0b00100100, 0b00001101};
    const size_t testoffset{4};
    const size_t CODE_SIZE = 9;
    std::vector<klzw::code_t> output{};
    size_t offset{};
    size_t tmp{};
    ssize_t tmp2{CODE_SIZE};
    klzw::details::BytesToCodes(testinput, &offset, &tmp, &tmp2, output, CODE_SIZE);

    ASSERT_EQ(testoutput.size(), output.size()) << "Vectors testoutput and output are of unequal length";
    EXPECT_EQ(testoffset, offset) << "offsets are not equal";

    for (size_t i = 0; i < testoutput.size(); ++i)
    {
        EXPECT_EQ(testoutput[i], output[i]) << "Vectors testoutput and output differ at index " << i;
    }
}
