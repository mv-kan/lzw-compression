#include <iostream>
#include <bitset>
#include <vector>

using byte = unsigned char;
const byte MAX_BYTE = 0xFF;
const int CODE_SIZE = 10; // in bits
const int BITS_IN_BYTE = 8;



void PrototypeConvertCodesToBytes(const std::vector<size_t>& codes, std::vector<byte>& bytes) { 
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

void PrototypeConverBytesToCodes(const std::vector<byte> &bytes, std::vector<size_t> &codes)
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
        // if (codeSize > BITS_IN_BYTE) {
        //     ssize_t shift{static_cast<ssize_t>(byteoffset - offset)};
        //     codes[currentCodeIndex] += shift > 0 ? (size_t)bytes[i] << shift : (size_t)bytes[i] >> +shift;
        //     byteoffset += BITS_IN_BYTE;
        //     codeSize -= BITS_IN_BYTE;
        // }
        // else {
        //     codes[currentCodeIndex] += (size_t)bytes[i] & (0xFF >> +codeSize) << (byteoffset - offset);
        //     currentCodeIndex++;
        //     offset = BITS_IN_BYTE + codeSize;
        //     codes.push_back(0);
        //     i--;
        //     byteoffset = 0;
        //     codeSize = CODE_SIZE;
        // }
    }
}

int main() {
    std::vector<size_t> codes{ 0b110001010, 0b101010101, 0b100001111, 0b1, 0b1};
    std::vector<size_t> codes2;
    std::vector<byte> bytes;

    PrototypeConvertCodesToBytes(codes, bytes);

    for (size_t i = 0; i < bytes.size(); i++) {
        std::cout << std::bitset<8>(bytes[i]) << std::endl;
    }

    PrototypeConverBytesToCodes(bytes, codes2);

    for (size_t i = 0; i < codes2.size(); i++)
    {
        std::cout << std::bitset<10>(codes2[i]) << std::endl;
    }

    return 0;
}