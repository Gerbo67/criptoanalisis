#pragma once
#include "Prerequisites.h"

class XOREncoder {
public:
    /**
     * @brief Constructor.
     * @param dictionary_filepath Ruta al archivo de diccionario de claves para bruteForceByDictionary.
     */
    XOREncoder(const std::string& dictionary_filepath = "dictionaries/clavesXOR.txt") :
        dict_filepath_(dictionary_filepath) {
        std::cout << "INFO (XOR): XOREncoder inicializado. Ruta de diccionario: " << dict_filepath_ << std::endl;
    }

    ~XOREncoder() = default;

    /**
     * @brief Codifica/Decodifica el input usando XOR con la clave. La operación es simétrica.
     */
    std::string
    encode(const std::string& input, const std::string& key) {
        std::string output = input;
        if (key.empty()) {
            return input;
        }
        for (size_t i = 0; i < input.size(); i++) {
            output[i] = input[i] ^ key[i % key.size()];
        }
        return output;
    }

    /**
     * @brief Transforma un texto hexadecimal (bytes separados por espacios o sin ellos) a un vector de bytes.
     */
    std::vector<unsigned char>
    hexToBytes(const std::string& hex_input) {
        std::vector<unsigned char> bytes;
        std::string clean_hex_input;
        for (char c : hex_input) {
            if (!std::isspace(static_cast<unsigned char>(c))) {
                clean_hex_input += c;
            }
        }

        for (size_t i = 0; i + 1 < clean_hex_input.length(); i += 2) {
            std::string byte_str = clean_hex_input.substr(i, 2);
            try {
                unsigned long byte_val = std::stoul(byte_str, nullptr, 16);
                bytes.push_back(static_cast<unsigned char>(byte_val));
            } catch (const std::invalid_argument& ia) {
                std::cerr << "Error (hexToBytes): Valor hexadecimal inválido encontrado: " << byte_str << std::endl;
            } catch (const std::out_of_range& oor) {
                std::cerr << "Error (hexToBytes): Valor hexadecimal fuera de rango: " << byte_str << std::endl;
            }
        }
        return bytes;
    }

    /**
     * @brief Imprime un string como una secuencia de valores hexadecimales.
     */
    void
    printHex(const std::string& input) {
        if (input.empty()) {
            std::cout << "(vacío)" << std::endl;
            return;
        }
        for (unsigned char c : input) {
            std::cout << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(c) << " ";
        }
        std::cout << std::dec << std::endl;
    }

    /**
     * @brief Verifica si un string contiene principalmente caracteres imprimibles o de espacio.
     */
    bool
    isValidText(const std::string& data) {
        if (data.empty())
            return false;
        return std::all_of(data.begin(), data.end(), [](unsigned char c) {
            return std::isprint(c) || std::isspace(c); // isspace incluye espacio, tab, newline, etc.
        });
    }

    /**
     * @brief Realiza fuerza bruta con claves de 1 byte e imprime resultados válidos.
     * @param cifrado Vector de bytes del texto cifrado.
     */
    void
    bruteForce_1Byte(const std::vector<unsigned char>& cifrado) {
        std::cout << "\n--- Iniciando Fuerza Bruta XOR - Claves de 1 Byte ---" << std::endl;
        if (cifrado.empty()) {
            std::cout << "  Texto cifrado vacío, no se puede realizar fuerza bruta de 1 byte." << std::endl;
            return;
        }
        int valid_count = 0;
        for (int clave_byte = 0; clave_byte < 256; ++clave_byte) {
            std::string result_text;
            result_text.reserve(cifrado.size());
            for (unsigned char c_byte : cifrado) {
                result_text += static_cast<unsigned char>(c_byte ^ clave_byte);
            }

            if (isValidText(result_text)) {
                valid_count++;
                std::cout << "=============================\n";
                std::cout << "Clave Byte  : 0x" << std::hex << std::setw(2) << std::setfill('0') << clave_byte
                    << " ('" << (std::isprint(clave_byte) ? static_cast<char>(clave_byte) : '.') << "')\n";
                std::cout << "Texto posible : " << result_text << "\n";
            }
        }
        if (valid_count == 0) {
            std::cout << "  No se encontraron textos legibles con claves de 1 byte." << std::endl;
        }
        std::cout << std::dec;
    }

    /**
     * @brief Realiza fuerza bruta con claves de 2 bytes e imprime resultados válidos.
     * @param cifrado Vector de bytes del texto cifrado.
     */
    void
    bruteForce_2Byte(const std::vector<unsigned char>& cifrado) {
        std::cout << "\n--- Iniciando Fuerza Bruta XOR - Claves de 2 Bytes ---" << std::endl;
        if (cifrado.empty()) {
            std::cout << "  Texto cifrado vacío, no se puede realizar fuerza bruta de 2 bytes." << std::endl;
            return;
        }
        int valid_count = 0;
        for (int b1 = 0; b1 < 256; ++b1) {
            for (int b2 = 0; b2 < 256; ++b2) {
                std::string result_text;
                result_text.reserve(cifrado.size());
                unsigned char current_key[2] = {static_cast<unsigned char>(b1), static_cast<unsigned char>(b2)};
                for (size_t i = 0; i < cifrado.size(); ++i) {
                    result_text += cifrado[i] ^ current_key[i % 2];
                }

                if (isValidText(result_text)) {
                    valid_count++;
                    std::cout << "=============================\n";
                    std::cout << "Clave 2 bytes : 0x" << std::hex << std::setw(2) << std::setfill('0') << b1
                        << " 0x" << std::setw(2) << std::setfill('0') << b2
                        << " ('" << (std::isprint(b1) ? static_cast<char>(b1) : '.')
                        << (std::isprint(b2) ? static_cast<char>(b2) : '.') << "')\n";
                    std::cout << "Texto posible : " << result_text << "\n";
                }
            }
        }
        if (valid_count == 0) {
            std::cout << "  No se encontraron textos legibles con claves de 2 bytes." << std::endl;
        }
        std::cout << std::dec;
    }

    /**
     * @brief Carga claves desde un archivo de texto.
     * @param filepath Ruta al archivo de diccionario.
     * @return Un vector de strings con las claves cargadas.
     */
    std::vector<std::string>
    loadKeysFromFile(const std::string& filepath) {
        std::vector<std::string> keys;
        std::ifstream file(filepath);
        std::string line;

        if (!file.is_open()) {
            std::cerr << "ADVERTENCIA (XOR): No se pudo abrir el archivo de diccionario: " << filepath
                << ". Usando lista de claves por defecto." << std::endl;
            return {"clave", "admin", "1234", "secret", "pass", "test", "default"};
        }

        while (std::getline(file, line)) {
            line.erase(0, line.find_first_not_of(" \t\n\r\f\v"));
            line.erase(line.find_last_not_of(" \t\n\r\f\v") + 1);
            if (!line.empty()) {
                keys.push_back(line);
            }
        }
        file.close();

        if (keys.empty()) {
            std::cerr << "ADVERTENCIA (XOR): El archivo de diccionario '" << filepath
                << "' está vacío o no contiene claves válidas. Usando lista de claves por defecto." << std::endl;
            return {"clave", "admin", "1234", "secret", "pass", "test", "default"};
        }
        std::cout << "INFO (XOR): Cargadas " << keys.size() << " claves desde '" << filepath << "'" << std::endl;
        return keys;
    }

    /**
     * @brief Realiza fuerza bruta usando una lista de claves comunes cargadas desde un archivo.
     * Imprime los resultados válidos localmente.
     * @param cifrado Vector de bytes del texto cifrado.
     */
    void
    bruteForceByDictionary(const std::vector<unsigned char>& cifrado) {
        std::cout << "\n--- Iniciando Fuerza Bruta XOR - Por Diccionario (desde archivo) ---" << std::endl;
        if (cifrado.empty()) {
            std::cout << "  Texto cifrado vacío, no se puede realizar ataque por diccionario." << std::endl;
            return;
        }

        std::vector<std::string> clavesComunes = loadKeysFromFile(dict_filepath_);

        if (clavesComunes.empty()) {
            std::cout <<
                "INFO (XOR): No se cargaron claves del diccionario o la lista de fallback está vacía. El ataque por diccionario no puede continuar."
                << std::endl;
            return;
        }
        int valid_count = 0;
        for (const auto& clave_str : clavesComunes) {
            if (clave_str.empty())
                continue;
            std::string result_text;
            result_text.reserve(cifrado.size());
            for (size_t i = 0; i < cifrado.size(); ++i) {
                result_text += static_cast<unsigned char>(cifrado[i] ^ clave_str[i % clave_str.size()]);
            }

            if (isValidText(result_text)) {
                valid_count++;
                std::cout << "=============================\n";
                std::cout << "Clave de diccionario: '" << clave_str << "'\n";
                std::cout << "Texto posible : " << result_text << "\n";
            }
        }
        if (valid_count == 0) {
            std::cout << "  No se encontraron textos legibles con las claves del diccionario proporcionado." <<
                std::endl;
        }
    }

private:
    std::string dict_filepath_;
};
