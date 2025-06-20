#pragma once
#include "EvaluationIA.h"
#include "Prerequisites.h"

class
    CesarEncryption {
public:
    CesarEncryption(bool use_api_for_evaluation = false) :
        use_api_mode_(use_api_for_evaluation) {
        try {
            api_evaluator_ = std::make_unique<EvaluationIA>();
        } catch (const std::exception& e) {
            api_evaluator_ = nullptr;
        }
    }

    ~CesarEncryption() = default;

    /**
     * @brief Codifica un texto usando el cifrado César.
     * @param texto El texto a codificar.
     * @param desplazamiento El número de posiciones a desplazar (clave).
     * @return El texto codificado.
     */
    std::string
    encode(const std::string& texto, int desplazamiento) {
        std::string result = "";
        int letter_shift = (desplazamiento % 26 + 26) % 26;
        int digit_shift = (desplazamiento % 10 + 10) % 10;

        for (char c : texto) {
            if (c >= 'A' && c <= 'Z') {
                result += (char)(((c - 'A' + letter_shift) % 26) + 'A');
            } else if (c >= 'a' && c <= 'z') {
                result += (char)(((c - 'a' + letter_shift) % 26) + 'a');
            } else if (c >= '0' && c <= '9') {
                result += (char)(((c - '0' + digit_shift) % 10) + '0');
            } else {
                result += c;
            }
        }
        return result;
    }

    /**
     * @brief Decodifica un texto cifrado con César.
     * @param texto El texto cifrado.
     * @param desplazamiento La clave original usada para codificar.
     * @return El texto decodificado.
     */
    std::string
    decode(const std::string& texto, int desplazamiento) {
        int letter_shift_decode = (26 - (desplazamiento % 26)) % 26;
        return encode(texto, letter_shift_decode);
    }

    /**
     * @brief Muestra todos los 26 posibles descifrados (ataque de fuerza bruta para el componente alfabético).
     * @param texto El texto cifrado.
     */
    void
    bruteForceAttack(const std::string& texto) {
        std::cout << "\nIntentos de descifrado por fuerza bruta (clave alfabética 0-25):\n";
        for (int clave_original = 0; clave_original < 26; ++clave_original) {
            std::string intento = decode(texto, clave_original);
            std::cout << "Clave original supuesta " << clave_original << ": " << intento << std::endl;
        }
    }

    /**
     * @brief Evalúa y devuelve la clave más probable para un texto cifrado.
     * Si está en modo API, también invoca al evaluador de congruencia externo.
     * Imprime los 3 mejores resultados del análisis local (texto, clave, score).
     * @param texto El texto cifrado.
     * @return La clave (desplazamiento original 0-25) más probable según el análisis local.
     */
    int
    evaluatePossibleKey(const std::string& texto) {
        if (use_api_mode_ && api_evaluator_) {
            std::cout << "\n--- Evaluacion usando API de Congruencia Externa ---" << std::endl;
            std::vector<std::string> todos_los_descifrados;
            std::vector<std::string> identificadores_clave; // Para las claves originales

            for (int clave_original = 0; clave_original < 26; ++clave_original) {
                todos_los_descifrados.push_back(decode(texto, clave_original));
                // El identificador será la clave original como string
                identificadores_clave.push_back("Clave: " + std::to_string(clave_original));
            }
            // Llama a la API con los textos y sus respectivos identificadores de clave
            api_evaluator_->evaluate_and_print_top_three(todos_los_descifrados, identificadores_clave);
            // La API ahora imprimirá los 3 mejores textos junto con su "ID/Recorrido" (la clave original).
        } else if (use_api_mode_ && !api_evaluator_) {
            std::cout << "\nADVERTENCIA: Modo API solicitado pero CongruenceEvaluator no está disponible. "
                << "Realizando solo análisis local." << std::endl;
        }

        std::cout << "\n--- Evaluacion con Analisis de Frecuencia Local ---" << std::endl;
        return claveLocal(texto, true);
    }

private:
    bool use_api_mode_;
    std::unique_ptr<EvaluationIA> api_evaluator_;

    /**
     * @brief Implementación del análisis de clave local basado en frecuencia de letras y palabras comunes.
     * @param texto El texto cifrado.
     * @param print_top_three Si es true, imprime los 3 resultados locales más probables.
     * @return La clave (0-25) más probable según este análisis.
     */
    int
    claveLocal(const std::string& texto, bool print_top_three) {
        struct KeyScore {
            int key;
            int score;
            std::string decoded_text;
            bool operator<(const KeyScore& other) const { return score > other.score; }
        };
        std::vector<KeyScore> possible_keys;
        const std::vector<std::string> comunes = {
            "de", "la", "que", "el", "en", "y", "a", "los", "se", "del", "las",
            "un", "por", "con", "no", "una", "su", "para", "es", "al", "lo", "como",
            "mas", "o", "pero", "sus", "le", "ha", "me", "si", "sin", "sobre", "este",
            "ya", "entre", "cuando", "todo", "esta", "ser", "son", "dos", "tambien", "fue", "habia",
            "era", "muy", "anos", "hasta", "desde", "gran", "esto", "nos", "mi", "mucho", "usted"};

        for (int clave_original = 0; clave_original < 26; ++clave_original) {
            std::string descifrado = decode(texto, clave_original);
            int puntaje = 0;
            std::string descifrado_lower = descifrado;
            std::transform(descifrado_lower.begin(), descifrado_lower.end(), descifrado_lower.begin(),
                           [](unsigned char c) { return std::tolower(c); });

            for (const std::string& palabra : comunes) {
                if (descifrado_lower.find(palabra) != std::string::npos) {
                    puntaje += palabra.length();
                }
            }
            puntaje += static_cast<int>(std::count(descifrado.begin(), descifrado.end(), ' '));
            possible_keys.push_back({clave_original, puntaje, descifrado});
        }

        if (possible_keys.empty()) {
            if (print_top_three) {
                std::cout << "  No se pudo realizar el análisis de frecuencia local (sin resultados)." << std::endl;
            }
            return 0;
        }

        std::sort(possible_keys.begin(), possible_keys.end());

        if (print_top_three) {
            std::cout << "Mejores 3 claves segun analisis local (clave y movimiento son el mismo valor aqui):"
                << std::endl;
            for (int i = 0; i < std::min((int)possible_keys.size(), 3); ++i) {
                std::cout << "  " << i + 1 << ". Clave/Movimiento: " << possible_keys[i].key
                    << ", Score Local: " << possible_keys[i].score << ", Texto: \""
                    << (possible_keys[i].decoded_text.length() > 60
                        ? possible_keys[i].decoded_text.substr(0, 60) + "..."
                        : possible_keys[i].decoded_text)
                    << "\"" << std::endl;
            }
        }
        return possible_keys[0].key;
    }
};
