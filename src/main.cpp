#include <iostream>
#include <bitset>
#include <vector>
#include <string>
#include <fstream>
#include "comptable.h"
#include "klzw.h"
// klzw stands for kan's lzw 
using byte = unsigned char;
const byte MAX_BYTE = 0xFF;
const int CODE_SIZE = 9; // in bits
const int BITS_IN_BYTE = 8;



void ProtoConvertCodesToBytes(const std::vector<size_t>& codes, std::vector<byte>& bytes) { 
    size_t offset{};
    size_t currentByteIndex{};
    if (bytes.size() == 0)
        bytes.push_back(0);

    for (size_t i = 0; i < codes.size(); ++i) {
        size_t code = codes[i];
        ssize_t codeSize = CODE_SIZE;
        while (code || codeSize > 0) {
            bytes[currentByteIndex] += (code << offset) & MAX_BYTE;
            code >>= BITS_IN_BYTE - offset;
            codeSize -= BITS_IN_BYTE - offset;
            // if code size is bigger than 0 then we just add another byte 
            if (codeSize > 0) {
                bytes.push_back(0);
                currentByteIndex++;

                // we dont have offset here so we reset it 
                offset = 0;
            } else { // otherwise we change offset value,
                offset = BITS_IN_BYTE + codeSize; // this is subtraction because codeSize is negative here
            }
        }
    }
}

void ProtoConverBytesToCodes(const std::vector<byte> &bytes, std::vector<size_t> &codes)
{
    if (codes.size() == 0)
        codes.push_back(0);

    size_t offset{};
    size_t currentCodeIndex{};
    ssize_t codeSize{CODE_SIZE};
    size_t byteshift{};

    for (size_t i = 0; i < bytes.size(); i++)
    {
        size_t byte = static_cast<size_t>(bytes[i]);
        if (codeSize < BITS_IN_BYTE)
            byte &= MAX_BYTE >> (BITS_IN_BYTE - codeSize - offset);
        codes[currentCodeIndex] += (byte << byteshift) >> offset;
        byteshift += BITS_IN_BYTE;
        codeSize -= BITS_IN_BYTE;

        // reset and parse next code
        if (codeSize < 0) {
            currentCodeIndex++;
            codes.push_back(0);
            // codeSize - is negative here  
            offset = (BITS_IN_BYTE + codeSize + offset) % BITS_IN_BYTE;
            codeSize = CODE_SIZE;
            byteshift = 0;
            // if we have offset then we have some bits to read in this iteration
            // otherwise we have read all bits in "i" byte
            if (offset > 0)
                i--;
        }
    }
}

// reads codes that was writen by ProtoWriteToFileCodes file
void ProtoReadFromFileConvertToCodes(const std::string file, std::vector<size_t> &codes)
{
    std::ifstream in(file, std::ios::in | std::ios::binary); // or use full path "C:/example.txt"
    if (in.is_open())
    {
        byte buf[100];
        in.read(reinterpret_cast<char*>(buf), sizeof(buf));
        std::vector<byte> bytes{buf, buf + sizeof(buf) / sizeof(buf[0])};
        // size_t eof_code = 0b1111111111;
        ProtoConverBytesToCodes(bytes, codes);
        // for (size_t i = 0; i < codes.size(); i++)
        // {
        //     if (codes[i] == eof_code) {
        //         codes.resize(i + 1);
        //         break;
        //     }
        // }
    }
    else
    {
        std::cout << "Unable to open file.\n";
    }
}

// write codes 
void ProtoWriteToFileCodes(const std::string file, const std::vector<size_t> &codes)
{
    std::vector<byte> bytes{};
    ProtoConvertCodesToBytes(codes, bytes);
    std::ofstream out;
    out.open(file, std::ios::out | std::ios::binary); // or use full path "C:/example.bin"
    out.write(reinterpret_cast<const char *>(& bytes[0]), bytes.size());
    out.close();
}

int main() {
    std::cout << "test of reading file" << std::endl;
    klzw::Compress("/home/kan/Dev/lzw-compression/build/filecomptest.bin", "/home/kan/Dev/lzw-compression/build/filecomptest2.bin.klzw");

    return 0;
    // klzw::details::comptable tb{};
    // std::cout << "Hi" << std::endl;
    // std::cout << "tb['A'] = " << tb.Get({'A'}) << std::endl;
    // std::cout << "tb['a'] = " << tb.Get({'a'}) << std::endl;
    // std::cout << "tb['B'] = " << tb.Get({'B'}) << std::endl;
    // std::cout << "tb['b'] = " << tb.Get({'b'}) << std::endl;
    // std::cout << "tb['\\null'] = " << tb.Get({0}) << std::endl;
    // return 0;
    std::vector<size_t> codes{0b110001010, 0b101010101, 0b010001111, 0b1, 0b1, 0b111111101, 0b111111100, 0b101001001};
    std::vector<size_t> codes2;
    std::vector<byte> bytes;

    size_t offset = klzw::details::CodesToBytes(codes, CODE_SIZE, bytes, 2);
    std::cout << offset << std::endl;
    for (size_t i = 0; i < bytes.size(); i++) {
        // std::cout << std::bitset<8>(bytes[i]) << "\t" << (int)bytes[i] << std::endl;
        std::cout << "0b"<<std::bitset<8>(bytes[i]) << ", ";
    }
    std::cout << offset << std::endl;
    return 0;
    ProtoConverBytesToCodes(bytes, codes2);

    for (size_t i = 0; i < codes2.size(); i++)
    {
        std::cout << std::bitset<CODE_SIZE>(codes2[i]) << std::endl;
    }
    codes2.resize(0);

    ProtoWriteToFileCodes("./test.bin", codes);
    ProtoReadFromFileConvertToCodes("./test.bin", codes2);
    std::cout << "After read\n";
    for (size_t i = 0; i < codes2.size(); i++)
    {
        std::cout << std::bitset<10>(codes2[i]) << std::endl;
    }
    return 0;
}