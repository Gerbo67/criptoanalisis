#include "Prerequisites.h"
#include "XOREncoder.h"

int main() {
  XOREncoder XORencoder;

  std::string mensaje = "Hola Mundo";
  std::string clave = "clave";
  
  std::cout << "Mensaje original: " << mensaje << std::endl;

  std::cout << std::endl;
  std::cout << std::endl;
  std::string cifrado = XORencoder.encode(mensaje, clave);

  std::cout << "Texto cifrado (original): " << cifrado << std::endl;
  std::string descifrado = XORencoder.encode(cifrado, clave);


  std::cout << "Texto cifrado (hex): "; 
  XORencoder.printHex(cifrado);
  std::cout << std::endl;

  std::cout << "Mensaje descifrado: " << descifrado << std::endl;

  std::vector<unsigned char> bytesCifrados(cifrado.begin(), cifrado.end());

  std::cout << "\n--- Fuerza bruta (1 byte) con filtro ---\n";
 // XORencoder.bruteForce_1Byte(bytesCifrados);

  std::cout << "\n--- Fuerza bruta (2 bytes) con filtro ---\n";
 // XORencoder.bruteForce_2Byte(bytesCifrados);

  std::cout << "\n--- Fuerza bruta (diccionario de claves) ---\n";
  XORencoder.bruteForceByDictionary(bytesCifrados);
  return 0;
}
