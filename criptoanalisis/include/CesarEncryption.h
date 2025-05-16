#pragma once
#include <iostream>
#include <string>

class
	CesarEncryption {
public:
	CesarEncryption() = default;
	~CesarEncryption() = default;

	std::string
		encode(const std::string& texto, int desplazamiento) {
		std::string result = "";

		for (char c : texto) {
			if (c >= 'A' && c <= 'Z') {
				result += (char)(((c - 'A' + desplazamiento) % 26) + 'A');
			}
			else if (c >= 'a' && c <= 'z') {
				result += (char)(((c - 'a' + desplazamiento) % 26) + 'a');
			}
			else if (c >= '0' && c <= '9') {
				result += (char)(((c - '0' + desplazamiento) % 10) + '0');
			}
			else {
				result += c;
			}
		}

		return result;
	}

	std::string
		decode(const std::string& texto, int desplazamiento) {
		return encode(texto, 26 - (desplazamiento % 26));
	}

	void
		bruteForceAttack(const std::string& texto) {
		std::cout << "\nIntentos de descifrado por fuerza bruta:\n";
		for (int clave = 0; clave < 26; clave++) {
			std::string intento = encode(texto, 26 - clave);
			std::cout << "Clave " << clave << ": " << intento << std::endl;
		}
	}

	int
	evaluatePossibleKey(const std::string& texto) {
		int frecuencias[26] = { 0 };

		// Contar frecuencias de letras
		for (char c : texto) {
			if (c >= 'a' && c <= 'z') {
				frecuencias[c - 'a']++;
			}
			else if (c >= 'A' && c <= 'Z') {
				frecuencias[c - 'A']++;
			}
		}

		// Letras comunes en español (orden de frecuencia)
		const char letrasEsp[] = { 'e', 'a', 'o', 's', 'r', 'n', 'i', 'd', 'l', 'c' };

		// Buscar la letra más frecuente en el mensaje cifrado
		int indiceMax = 0;
		for (int i = 1; i < 26; ++i) {
			if (frecuencias[i] > frecuencias[indiceMax]) {
				indiceMax = i;
			}
		}

		// Probar varias suposiciones de mapeo
		int mejorClave = 0;
		int mejorPuntaje = -1;

		for (char letraRef : letrasEsp) {
			int clave = (indiceMax - (letraRef - 'a') + 26) % 26;
			int puntaje = 0;

			// Descifrar con esa clave
			std::string descifrado = encode(texto, 26 - clave);

			// Palabras comunes simples
			std::string comunes[] = { "el", "de", "la", "que", "en", "y", "los", "se" };

			for (std::string palabra : comunes) {
				if (descifrado.find(palabra) != std::string::npos) {
					puntaje++;
				}
			}

			if (puntaje > mejorPuntaje) {
				mejorPuntaje = puntaje;
				mejorClave = clave;
			}
		}

		return mejorClave;
	}

private:

};