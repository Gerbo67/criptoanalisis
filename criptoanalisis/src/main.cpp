#include <iostream>
#include <string>
#include "CesarEncryption.h"
using namespace std;

string cifrarCesar(const string& texto, int rotacion) {
  string resultado = "";

  for (char c : texto) {
    if (c >= 'A' && c <= 'Z') {
      resultado += (char)(((c - 'A' + rotacion) % 26) + 'A');
    }
    else if (c >= 'a' && c <= 'z') {
      resultado += (char)(((c - 'a' + rotacion) % 26) + 'a');
    }
    else if (c >= '0' && c <= '9') {
      resultado += (char)(((c - '0' + rotacion) % 10) + '0');
    }
    else {
      resultado += c;
    }
  }

  return resultado;
}

string descifrarCesar(const string& texto, int rotacion) {
  return cifrarCesar(texto, 26 - (rotacion % 26));
}

void ataqueFuerzaBruta(const string& cifrado) {
  cout << "\nIntentos de descifrado por fuerza bruta:\n";
  for (int clave = 1; clave < 26; ++clave) {
    string intento = cifrarCesar(cifrado, 26 - clave);
    cout << "Clave " << clave << ": " << intento << endl;
  }
}

int calcularClaveProbable(const string& texto) {
  int frecuencias[26] = { 0 };

  for (char c : texto) {
    if (c >= 'a' && c <= 'z') {
      frecuencias[c - 'a']++;
    }
    else if (c >= 'A' && c <= 'Z') {
      frecuencias[c - 'A']++;
    }
  }

  int indiceMax = 0;
  for (int i = 1; i < 26; ++i) {
    if (frecuencias[i] > frecuencias[indiceMax]) {
      indiceMax = i;
    }
  }

  int claveProbable = (indiceMax - ('e' - 'a') + 26) % 26;
  return claveProbable;
}


// Cifrado: Fmirzirmhsw e pe gpewi hi wikyvmheh teve zmhisnyiksw. Ir iwxe qexivme, etvirhiver e gmjvev qirweniw c hiwgmjvev gshmksw sgypxsw
int main() {
  string mensaje = "Bienvenidos a la clase de seguridad para videojuegos. En esta materia, aprenderan a cifrar mensajes y descifrar codigos ocultos";
  int rotacion = 4;
  /*

  string cifrado = cifrarCesar(mensaje, rotacion);

  string descifrado = descifrarCesar(cifrado, rotacion);

  cout << "Posible Clave:" + std::to_string( calcularClaveProbable(cifrado)) << endl;
  */

  CesarEncryption cesar;
  string cifrado = cesar.encode(mensaje, rotacion);
  cout << "Texto cifrado: " << cifrado << endl;
  string descifrado = cesar.decode(cifrado, rotacion);
  cout << "Texto descifrado: " << descifrado << endl;
  //cesar.bruteForceAttack(cifrado);
  cout << "Posible Clave:" + std::to_string( cesar.evaluatePossibleKey(cifrado)) << endl;
  return 0;
}
