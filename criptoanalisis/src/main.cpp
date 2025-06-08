#include "Prerequisites.h"
#include "AsciiBinary.h"
#include "CesarEncryption.h"
#include "DES.h"
#include "XOREncoder.h"

void
useAscii() {
    std::cout << "--- DEMOSTRACIÓN DE AsciiBinary ---" << std::endl;

    AsciiBinary AB;
    std::string input_text = "Hello world";

    std::cout << "Texto Original   : " << input_text << std::endl;

    std::string binary_output = AB.stringToBinary(input_text);
    std::cout << "Convertido a Binario: " << binary_output << std::endl;

    std::string text_from_binary = AB.binaryToString(binary_output);
    std::cout << "Convertido a texto: " << text_from_binary << std::endl;

    std::cout << "\n--- FIN DE LA DEMOSTRACIÓN ---" << std::endl;
}

void
useXOR() {
    std::cout << "--- DEMOSTRACIÓN DE XOREncoder ---" << std::endl;

    // --- 1. PRUEBA DE CODIFICACIÓN/DECODIFICACIÓN ---
    XOREncoder encoder;
    std::string mensaje_original = "Hola Mundo!";
    std::string clave_xor = "clave";
    std::cout << "Original: " << mensaje_original << " | Clave: " << clave_xor << std::endl;
    std::string texto_cifrado_str = encoder.encode(mensaje_original, clave_xor);
    std::cout << "Cifrado (Hex): ";
    encoder.printHex(texto_cifrado_str);
    std::string texto_descifrado_str = encoder.encode(texto_cifrado_str, clave_xor);
    std::cout << "Descifrado: " << texto_descifrado_str << std::endl;

    // --- 2. PREPARANDO DATOS PARA FUERZA BRUTA ---
    std::string mensaje_para_fb = "Secreto";
    std::string clave_fb_real = "abc"; // Asegúrate que 'abc' esté en common_keys.txt
    std::string cifrado_fb_str = encoder.encode(mensaje_para_fb, clave_fb_real);
    std::cout << "\nOriginal para FB: " << mensaje_para_fb << " | Clave Real: " << clave_fb_real << std::endl;
    std::cout << "Cifrado para FB (Hex): ";
    encoder.printHex(cifrado_fb_str);
    std::vector<unsigned char> cifrado_fb_bytes(cifrado_fb_str.begin(), cifrado_fb_str.end());

    // --- 3. PRUEBAS DE FUERZA BRUTA ---
    // a) Fuerza Bruta con Claves de 1 Byte
    encoder.bruteForce_1Byte(cifrado_fb_bytes);

    // b) Fuerza Bruta por Diccionario (usando "common_keys.txt" por defecto)
    encoder.bruteForceByDictionary(cifrado_fb_bytes);

    std::cout << "\n--- FIN DE LA DEMOSTRACIÓN REDUCIDA ---" << std::endl;
}

void
useCesar(bool local) {
    if (local) {
        std::cout << "********** MODO LOCAL **********" << std::endl;
        CesarEncryption cesar_local_mode(false); // El 'false' es opcional (default)
        std::string mensaje_cifrado_local = cesar_local_mode.encode("Mensaje para prueba local de cifrado Cesar", 7);
        std::cout << "Cifrado (local): " << mensaje_cifrado_local << std::endl;
        int clave_local = cesar_local_mode.evaluatePossibleKey(mensaje_cifrado_local);
        std::cout << "Mejor clave estimada (local): " << clave_local << std::endl;
        std::cout << "Texto con clave estimada (local): " << cesar_local_mode.decode(mensaje_cifrado_local, clave_local)
            << std::endl;
        std::cout << "******************************\n\n";
    } else {
        std::cout << "********** MODO API + LOCAL **********" << std::endl;
        CesarEncryption cesar_api_mode(true);
        std::string mensaje_cifrado_api = cesar_api_mode.encode("Pero eso esperaba bajo la lluvia", 11);
        std::cout << "Cifrado (para API): " << (mensaje_cifrado_api.length() > 80
            ? mensaje_cifrado_api.substr(0, 80) + "..."
            : mensaje_cifrado_api) << std::endl;
        int clave_api_y_local = cesar_api_mode.evaluatePossibleKey(mensaje_cifrado_api);
        std::cout << "Mejor clave estimada (devuelta, basada en análisis local): " << clave_api_y_local << std::endl;
        std::cout << "Texto con clave estimada (local): " << cesar_api_mode.decode(
                mensaje_cifrado_api, clave_api_y_local)
            << std::endl;
        std::cout << "***********************************\n";
    }
}

void
useDes() {
	std::bitset<64> key("0001001100110100010101110111100110011011101111001101111111110001");

	std::string phrase = "$Hola DES!";

	while (phrase.size() % 8 != 0) {
		phrase += '\0';
	}

	std::vector<std::bitset<64>> cipherBlocks;
	std::cout << "Texto original: " << phrase << std::endl;
	std::cout << "Cifrado en hexadecimal:\n";

	// Cifrado con DES
	DES des(key);
	for (size_t i = 0; i < phrase.size(); i += 8) {
		std::string block = phrase.substr(i, 8);
		auto blockBits = des.stringToBitset64(block);
		auto encrypted = des.encode(blockBits);
		cipherBlocks.push_back(encrypted);
		std::cout << std::hex << std::uppercase << std::setw(16) << std::setfill('0') << encrypted.to_ullong() << " ";
	}

	std::cout << std::endl;

	// Descifrado con DES
	std::string output;
	for (const auto& block : cipherBlocks) {
		auto decrypted = des.decode(block);
		output += des.bitset64ToString(decrypted);
	}

	std::cout << "Texto descifrado: " << phrase << std::endl;
	//auto ciphertext = des.encode(plaintext);
}

int
main() {
    constexpr bool local = false;

    //useCesar(local);
    //useXOR();
    useAscii();

    return 0;
}
