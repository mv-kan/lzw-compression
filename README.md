# simple LZW Compressor (klzw)

This is my implementation of lzw compression heavily inspired by this article https://www.cs.cmu.edu/~cil/lzw.and.gif.txt

## Features and stack 

- `gtest` for testing
- `cmake` for building
- `c++11` as main lang
    - `STL` standard template lib for basic stuff
    - `fstream` for file write and read

## Biggest challenges durring implementation

1. Adding gtest. It is not easy and not intuitive at all. I have spend whole evening to understand how you can add gtest without cloning  googletest repo to my repo. 
2. Writing arbitrary bits. So in summary LZW compression is about code size. And code size can be 9 bits, 10 bits, 13 bits. The biggest challenging part is to write 9 bits codes to file. Thus I need to pack 9 bits codes into 8 bits bytes and read bytes into 9 bits codes. It sounds not that difficult, but believe me it has its own pitfalls.  