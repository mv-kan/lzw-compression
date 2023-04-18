#include <gtest/gtest.h>
#include <vector>
#include <fstream>
#include "../src/compress.h"
TEST(compress, CodesToBytesBaseCase)
{
    std::vector<klzw::code_t> testinput{0b110001010, 0b101010101, 0b010001111, 0b1, 0b1, 0b111111101, 0b111111100, 0b101001001};
    std::vector<klzw::byte> testoutput{0b10001010, 0b10101011, 0b00111110, 0b00001010, 0b00010000, 0b10100000, 0b00111111, 0b11111111, 0b10100100};
    const size_t testoffset{0};
    const size_t CODE_SIZE = 9;
    std::vector<klzw::byte> output{};
    size_t offset = klzw::details::CodesToBytes(testinput, CODE_SIZE, output, 0);

    EXPECT_EQ(testoutput.size(), output.size()) << "Vectors testoutput and output are of unequal length";
    EXPECT_EQ(testoffset, offset) << "offsets are not equal";

    for (size_t i = 0; i < testoutput.size(); ++i)
    {
        EXPECT_EQ(testoutput[i], output[i]) << "Vectors testoutput and output differ at index " << i;
    }
}

TEST(compress, CodesToBytesOffset)
{
    std::vector<klzw::code_t> testinput{0b110001010, 0b101010101, 0b010001111, 0b1, 0b1, 0b111111101, 0b111111100, 0b101001001};
    std::vector<klzw::byte> testoutput{0b00101000, 0b10101110, 0b11111010, 0b00101000, 0b01000000, 0b10000000, 0b11111110, 0b11111100, 0b10010011, 0b00000010};
    const size_t testoffsetoutput{2};
    const size_t testoffset{2};
    const size_t CODE_SIZE = 9;
    std::vector<klzw::byte> output{};
    size_t offset = klzw::details::CodesToBytes(testinput, CODE_SIZE, output, testoffset);

    EXPECT_EQ(testoutput.size(), output.size()) << "Vectors testoutput and output are of unequal length";
    EXPECT_EQ(testoffsetoutput, offset) << "offsets are not equal";

    for (size_t i = 0; i < testoutput.size(); ++i)
    {
        EXPECT_EQ(testoutput[i], output[i]) << "Vectors testoutput and output differ at index " << i;
    }
}

TEST(compress, FileCompression)
{
    std::string testfile = "filecomptest.bin";
    std::ofstream output_file(testfile, std::ios::binary); // Open file in binary mode
    if (output_file.is_open())
    {
        char bytes[257];
        for (int i = 0; i < 255; i ++) {
            bytes[i] = i;
        }
        bytes[255] = 'A';
        bytes[256] = 'A';
        output_file.write(bytes, 257); // Write data to file

        output_file.close();
    }
    else
    {
        std::cout << "Unable to open file.\n";
    }
}