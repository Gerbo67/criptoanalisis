#pragma once
#include "Prerequisites.h"

class 
XOREncoder {
public:
	XOREncoder() = default;
	~XOREncoder() = default;
	
	// Encodes the input string using XOR with the provided key. 
	// Input: The string to be encoded. -> Hola Mundo
	// Key: The key to be used for encoding. -> clave
	std::string 
	encode(const std::string& input, const std::string& key) {
		std::string output = input;
		for (int i = 0; i < input.size(); i++) {
			output[i] = input[i] ^ key[i % key.size()];			
		}
		return output;
	}

	// Transform text to hex representation.
	std::vector<unsigned char> 
		HexToBytes(const std::string &input) {
		std::vector<unsigned char> bytes;
		std::istringstream iss(input);
		std::string hexValue;

		while (iss >> hexValue)	{
			if (hexValue.size() == 1) {
				hexValue = "0" + hexValue;
			}
			unsigned int byte;
			std::stringstream ss;
			ss << std::hex << hexValue;
			ss >> byte;
			bytes.push_back(static_cast<unsigned char>(byte));
		}
		return bytes;
	}

	void
	printHex(const std::string& input) {
		for (unsigned char c : input)	{
			std::cout << std::hex << std::setw(2) << std::setfill('0') << (int)c << " ";
		}
	}

	bool isValidText(const std::string& data) {
		return std::all_of(data.begin(), data.end(), [](unsigned char c) {
			return std::isprint(c) || std::isspace(c) || c == '\n' || c == ' ';
			});
	}

	void
	bruteForce_1Byte(const std::vector<unsigned char>& cifrado) {
		for (int clave = 0; clave < 256; ++clave) {
			std::string result;

			for (unsigned char c : cifrado) {
				result += static_cast<unsigned char>( c ^ clave);
			}

			if (isValidText(result)) {
				std::cout << "=============================\n";
				std::cout << "Clave 1 byte  : '" << static_cast<char>(clave)
					<< "' (0x" << std::hex << std::setw(2) << std::setfill('0') << clave << ")\n";
				std::cout << "Texto posible : " << result << "\n";
			}
		}
	}

	void
	bruteForce_2Byte(const std::vector<unsigned char>& cifrado) {
		for (int b1 = 0; b1 < 256; ++b1) {
			for (int b2 = 0; b2 < 256; ++b2){
				std::string result;
				unsigned char key[2] = {static_cast<unsigned char>(b1), static_cast<unsigned char>(b2)};

				for (int i = 0; i < cifrado.size(); i++) {
					result += cifrado[i] ^ key[i % 2];
				}

				if (isValidText(result)) {
					std::cout << "=============================\n";
					std::cout << "Clave 2 bytes : '" << static_cast<char>(b1) << static_cast<char>(b2)
						<< "' (0x" << std::hex << std::setw(2) << std::setfill('0') << b1
						<< " 0x" << std::setw(2) << std::setfill('0') << b2 << ")\n";
					std::cout << "Texto posible : " << result << "\n";
				}

			}
		}
		
	}

	void bruteForceByDictionary(const std::vector<unsigned char>& cifrado) {
		std::vector<std::string> clavesComunes = {
			"clave", "admin", "1234", "root", "test", "abc", "hola", "user",
			"pass", "12345", "0000", "password", "default"
		};

		for (const auto& clave : clavesComunes) {
			std::string result;
			for (int i = 0; i < cifrado.size(); i++) {
				result += static_cast<unsigned char>( cifrado[i] ^ clave[i % clave.size()]);
			}
			if (isValidText(result)) {
				std::cout << "=============================\n";
				std::cout << "Clave de diccionario: '" << clave << "'\n";
				std::cout << "Texto posible : " << result << "\n";
			}
		}
	}
private:

};