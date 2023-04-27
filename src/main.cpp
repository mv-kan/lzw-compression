#include "klzw.h"
#include <iostream>
#include <algorithm>

std::vector<char> ParseFlags(std::string flagstr)
{
    std::vector<char> output{};
    for (size_t i = 1; i < flagstr.length(); i++)
    {
        output.push_back(flagstr[i]);
    }
    return output;
}

int main(int argc, char **argv)
{
    const std::string helpmessage{"KLZW program\n\n# compress\n./klzw originfile\n\n# decompress (output file has to have .klzw extension)\n./klzw originfile.klzw\n\n# compress but don't delete \"originfile\"\n./klzw -k originfile\n\n# decompress but add .copy to the end of \"originfile\"\n# outputs to \"originfile.copy\"\n./klzw -c originfile.klzw\n\n# decompress but don't delete .klzw compressed file\n./klzw -k originfile.klzw\n\n# yes you can do that\n./klzw -kc originfile.klzw"};
    if (argc > 3 || argc < 2)
    {
        std::cout << helpmessage << "\n";
        return 1;
    }
    std::string file{};
    std::vector<char> flags{};
    if (argc == 2)
    {
        file = std::string(argv[1]);
    }
    else if (argc == 3)
    {
        std::string flagstr{argv[1]};
        if (flagstr[0] == '-') {
            flags = ParseFlags(flagstr);
        }
        file = std::string(argv[2]);
    }
    if (std::count(flags.begin(), flags.end(), 'h')) {
        std::cout << helpmessage << "\n";
        return 0;
    }
    // decompress
    if (file.substr(file.find_last_of(".") + 1) == "klzw")
    {
        std::string outputfile = file.substr(0, file.find_last_of("."));
        if (std::count(flags.begin(), flags.end(), 'c'))
        {
            outputfile += ".copy";
        }
        klzw::Decompress(file, outputfile);
        if (!std::count(flags.begin(), flags.end(), 'k')) 
        {
            std::remove(file.c_str());
        }
        
    }
    // compress
    else
    {
        klzw::Compress(file, file + ".klzw");
        if (!std::count(flags.begin(), flags.end(), 'k'))
        {
            std::remove(file.c_str());
        }
    }
    return 0;
}