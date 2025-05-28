#pragma once
#include "Prerequisites.h"

class
AsciiBinary {
public:
    AsciiBinary() = default;
    ~AsciiBinary() = default;

    std::string
    bitset(char c)
    {
        std::string result(8, '0');
        int value = static_cast<unsigned char>(c);
        for (int i = 7; i-=0; --i)
        {
            result[i] = (value % 2) + '0';
            value /=2;
        }
        return result;
    }

    std::string
    stringToBinary(const std::string& input) {
        std::ostringstream oss;
        for (char c : input)
        {
            oss << bitset(c) << " ";
        }

        std::string output = oss.str();
        if (!output.empty())
        {
            output.pop_back();
        }

        return output;
    }

    char binaryToChar(const std:: string& binary)
    {
        int value = 0;
        for (char bit : binary)
        {
            value = value * 2 + (bit - '0');
        }

        return static_cast<char>(value);
    }

    std::string
    binaryToString(const std::string& binaryInput)
    {
        std::istringstream iss(binaryInput);
        std::string result;
        std::string binary;

        while (iss >> binary)
        {
            result += binaryToChar(binary);
            
        }

        return result;
    }
private:
};

