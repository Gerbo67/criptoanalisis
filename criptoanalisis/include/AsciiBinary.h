#pragma once
#include "Prerequisites.h"

class
AsciiBinary {
public:
    AsciiBinary() = default;
    ~AsciiBinary() = default;

    /**
     * @brief Convierte un carácter a su representación binaria de 8 bits.
     * @param c El carácter a convertir.
     * @return Un string representando el carácter en binario.
     */
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

    /**
     * @brief Convierte un string de caracteres a su representación binaria.
     * Cada carácter se convierte a binario de 8 bits, separados por espacios.
     * @param input El string de entrada.
     * @return Un string con la representación binaria del string de entrada.
     */
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

    /**
     * @brief Convierte un string de binario (8 bits) a un carácter.
     * @param binary El string binario (debe tener 8 bits).
     * @return El carácter correspondiente.
     */
    char binaryToChar(const std:: string& binary)
    {
        int value = 0;
        for (char bit : binary)
        {
            value = value * 2 + (bit - '0');
        }

        return static_cast<char>(value);
    }

    /**
     * @brief Convierte un string de representaciones binarias (separadas por espacios) a un string de caracteres.
     * @param binaryInput El string de entrada con valores binarios.
     * @return El string de caracteres decodificado.
     */
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
