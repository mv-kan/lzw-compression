#include <gtest/gtest.h>
#include <vector>
#include <fstream>
#include "../src/compress.h"
#include <iterator>
TEST(compress, CodesToBytesBaseCase)
{
    std::vector<klzw::code_t> testinput{0b110001010, 0b101010101, 0b010001111, 0b1, 0b1, 0b111111101, 0b111111100, 0b101001001};
    std::vector<klzw::byte> testoutput{0b10001010, 0b10101011, 0b00111110, 0b00001010, 0b00010000, 0b10100000, 0b00111111, 0b11111111, 0b10100100};
    const size_t testoffset{0};
    const size_t CODE_SIZE = 9;
    std::vector<klzw::byte> output{};
    size_t offset = klzw::details::CodesToBytes(testinput, CODE_SIZE, output, 0);

    ASSERT_EQ(testoutput.size(), output.size()) << "Vectors testoutput and output are of unequal length";
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

TEST(compress, CodesToBytesExtendCodeCase)
{
    std::vector<klzw::code_t> testinput{0b110001010, 0b101010101, 0b010001111, 0b1, 0b1, 0b111111110, 0b1111111100, 0b1, 0b1101001001};
    std::vector<klzw::byte> testoutput{0b10001010, 0b10101011, 0b00111110, 0b00001010, 0b00010000, 0b11000000, 0b00111111, 0b11111111, 0b00000001, 0b00100100, 0b00001101};
    const size_t testoffsetoutput{4};
    const size_t testoffset{0};
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
        char bytes[258];
        for (int i = 0; i <= 255; i++)
        {
            bytes[i] = i;
        }
        bytes[256] = 'A';
        bytes[257] = 'A';
        output_file.write(bytes, 258); // Write data to file
        output_file.close();
        std::vector<klzw::byte> trueoutput{0b00000000, 0b00000010, 0b00001000, 0b00011000, 0b01000000, 0b10100000, 0b10000000, 0b10000001, 0b00000011, 0b00001000, 0b00010010, 0b00101000, 0b01011000, 0b11000000, 0b10100000, 0b10000001, 0b10000011, 0b00000111, 0b00010000, 0b00100010, 0b01001000, 0b10011000, 0b01000000, 0b10100001, 0b10000010, 0b10000101, 0b00001011, 0b00011000, 0b00110010, 0b01101000, 0b11011000, 0b11000000, 0b10100001, 0b10000011, 0b10000111, 0b00001111, 0b00100000, 0b01000010, 0b10001000, 0b00011000, 0b01000001, 0b10100010, 0b10000100, 0b10001001, 0b00010011, 0b00101000, 0b01010010, 0b10101000, 0b01011000, 0b11000001, 0b10100010, 0b10000101, 0b10001011, 0b00010111, 0b00110000, 0b01100010, 0b11001000, 0b10011000, 0b01000001, 0b10100011, 0b10000110, 0b10001101, 0b00011011, 0b00111000, 0b01110010, 0b11101000, 0b11011000, 0b11000001, 0b10100011, 0b10000111, 0b10001111, 0b00011111, 0b01000000, 0b10000010, 0b00001000, 0b00011001, 0b01000010, 0b10100100, 0b10001000, 0b10010001, 0b00100011, 0b01001000, 0b10010010, 0b00101000, 0b01011001, 0b11000010, 0b10100100, 0b10001001, 0b10010011, 0b00100111, 0b01010000, 0b10100010, 0b01001000, 0b10011001, 0b01000010, 0b10100101, 0b10001010, 0b10010101, 0b00101011, 0b01011000, 0b10110010, 0b01101000, 0b11011001, 0b11000010, 0b10100101, 0b10001011, 0b10010111, 0b00101111, 0b01100000, 0b11000010, 0b10001000, 0b00011001, 0b01000011, 0b10100110, 0b10001100, 0b10011001, 0b00110011, 0b01101000, 0b11010010, 0b10101000, 0b01011001, 0b11000011, 0b10100110, 0b10001101, 0b10011011, 0b00110111, 0b01110000, 0b11100010, 0b11001000, 0b10011001, 0b01000011, 0b10100111, 0b10001110, 0b10011101, 0b00111011, 0b01111000, 0b11110010, 0b11101000, 0b11011001, 0b11000011, 0b10100111, 0b10001111, 0b10011111, 0b00111111, 0b10000000, 0b00000010, 0b00001001, 0b00011010, 0b01000100, 0b10101000, 0b10010000, 0b10100001, 0b01000011, 0b10001000, 0b00010010, 0b00101001, 0b01011010, 0b11000100, 0b10101000, 0b10010001, 0b10100011, 0b01000111, 0b10010000, 0b00100010, 0b01001001, 0b10011010, 0b01000100, 0b10101001, 0b10010010, 0b10100101, 0b01001011, 0b10011000, 0b00110010, 0b01101001, 0b11011010, 0b11000100, 0b10101001, 0b10010011, 0b10100111, 0b01001111, 0b10100000, 0b01000010, 0b10001001, 0b00011010, 0b01000101, 0b10101010, 0b10010100, 0b10101001, 0b01010011, 0b10101000, 0b01010010, 0b10101001, 0b01011010, 0b11000101, 0b10101010, 0b10010101, 0b10101011, 0b01010111, 0b10110000, 0b01100010, 0b11001001, 0b10011010, 0b01000101, 0b10101011, 0b10010110, 0b10101101, 0b01011011, 0b10111000, 0b01110010, 0b11101001, 0b11011010, 0b11000101, 0b10101011, 0b10010111, 0b10101111, 0b01011111, 0b11000000, 0b10000010, 0b00001001, 0b00011011, 0b01000110, 0b10101100, 0b10011000, 0b10110001, 0b01100011, 0b11001000, 0b10010010, 0b00101001, 0b01011011, 0b11000110, 0b10101100, 0b10011001, 0b10110011, 0b01100111, 0b11010000, 0b10100010, 0b01001001, 0b10011011, 0b01000110, 0b10101101, 0b10011010, 0b10110101, 0b01101011, 0b11011000, 0b10110010, 0b01101001, 0b11011011, 0b11000110, 0b10101101, 0b10011011, 0b10110111, 0b01101111, 0b11100000, 0b11000010, 0b10001001, 0b00011011, 0b01000111, 0b10101110, 0b10011100, 0b10111001, 0b01110011, 0b11101000, 0b11010010, 0b10101001, 0b01011011, 0b11000111, 0b10101110, 0b10011101, 0b10111011, 0b01110111, 0b11110000, 0b11100010, 0b11001001, 0b10011011, 0b01000111, 0b10101111, 0b10011110, 0b10111101, 0b01111011, 0b11111000, 0b11110010, 0b11101001, 0b11011011, 0b11000111, 0b10101111, 0b10011111, 0b01111111, 0b01111111, 0b11111110, 0b00001001, 0b00100010, 0b00001000};

        klzw::Compress(testfile, testfile + ".klzw");

        std::ifstream outputfile(testfile + ".klzw", std::ios::binary);
        char buf[500];
        outputfile.read(buf, 500);
        ASSERT_EQ(trueoutput.size(), outputfile.gcount()) << "output and gcount does not have same value";
        size_t bytesread = static_cast<size_t>(outputfile.gcount());
        for (size_t i = 0; i < bytesread; i++)
        {
            EXPECT_EQ(trueoutput[i], static_cast<klzw::byte>(buf[i])) << "Vectors trueoutput and fileContents differ at index " << i;
        }
    }
    else
    {
        std::cout << "Unable to open file.\n";
    }
}