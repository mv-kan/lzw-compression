#include <iostream>
#include <bitset>
#include <vector>

using byte = unsigned char;
const byte MAX_BYTE = 0xFF;
const size_t CODE_SIZE = 10; // in bits
const size_t BITS_IN_BYTE = 8;



void ConvertCodesToBytes(const std::vector<size_t>& codes, std::vector<byte>& bytes) { 
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

void ConverBytesToCodes(const std::vector<byte> &bytes, std::vector<size_t> &codes)
{
}

int main() {
    std::vector<size_t> codes{ 0b110001010, 0b101010101, 0b100001111};
    std::vector<byte> bytes;

    ConvertCodesToBytes(codes, bytes);

    for (size_t i = 0; i < bytes.size(); i++) {
        std::cout << std::bitset<8>(bytes[i]) << std::endl;
    }
    return 0;
}