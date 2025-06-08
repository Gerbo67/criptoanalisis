#pragma once
#include "Prerequisites.h"

struct EvaluationResult {
    std::string text;
    double congruence_score;
    std::string identifier;

    bool operator<(const EvaluationResult& other) const {
        return congruence_score > other.congruence_score;
    }
};

class EvaluationIA {
public:
    EvaluationIA(const std::string& base_api_url = "http://localhost:8000",
                        const std::string& api_path = "/evaluar_congruencia/") :
        base_api_url_(base_api_url), api_path_(api_path) {
        try {
            cli_ = std::make_unique<httplib::Client>(base_api_url_);
            if (!cli_) {
                throw std::runtime_error("Fallo crítico: creación instancia httplib::Client.");
            }
            cli_->set_connection_timeout(5, 0);
            cli_->set_read_timeout(10, 0);
            cli_->set_write_timeout(10, 0);

            if (!cli_->is_valid()) {
                throw std::runtime_error(
                    "Error: URL API '" + base_api_url_ + "' inválida o fallo inicialización cliente HTTP.");
            }
        } catch (const std::exception& e) {
            std::cerr << "Error al inicializar CongruenceEvaluator: " << e.what() << std::endl;
            throw;
        }
    }

    /**
     * @brief Evalúa una lista de textos contra la API y muestra los 3 mejores resultados.
     * @param texts Un vector de strings, donde cada string es un texto a evaluar.
     * @param identifiers Un vector de strings con identificadores correspondientes a cada texto.
     * Debe tener el mismo tamaño y orden que 'texts'.
     */
    void
    evaluate_and_print_top_three(const std::vector<std::string>& texts,
                                      const std::vector<std::string>& identifiers) {
        if (!cli_ || !cli_->is_valid()) {
            std::cerr << "Error: Cliente HTTP no inicializado/configurado." << std::endl;
            return;
        }
        if (texts.size() != identifiers.size()) {
            std::cerr << "Error: El número de textos y de identificadores no coincide." << std::endl;
            return;
        }

        std::vector<EvaluationResult> all_results;
        nlohmann::json request_json_payload;

        for (size_t i = 0; i < texts.size(); ++i) {
            const auto& text_to_evaluate = texts[i];
            const auto& identifier_for_text = identifiers[i];

            request_json_payload["texto"] = text_to_evaluate;
            std::string request_body_str = request_json_payload.dump();

            auto res = cli_->Post(api_path_.c_str(), request_body_str, "application/json");

            if (res) {
                if (res->status == 200) {
                    try {
                        nlohmann::json response_json = nlohmann::json::parse(res->body);
                        if (response_json.contains("puntuacion_congruencia")) {
                            double score = response_json["puntuacion_congruencia"].get<double>();
                            all_results.push_back({text_to_evaluate, score, identifier_for_text});
                            // Incluye el identificador
                        } else {
                            std::cerr << "Warn: 'puntuacion_congruencia' no hallada para \"" << text_to_evaluate <<
                                "\". ID: " << identifier_for_text << ". Resp: " << (res->body.size() > 80
                                    ? res->body.substr(0, 80) + "..."
                                    : res->body) << std::endl;
                        }
                    } catch (const nlohmann::json::parse_error& e) {
                        std::cerr << "Error JSON parse (" << text_to_evaluate << ", ID: " << identifier_for_text <<
                            "): " << e.what() << ". Body: " << (res->body.size() > 80
                                ? res->body.substr(0, 80) + "..."
                                : res->body) << std::endl;
                    } catch (const nlohmann::json::type_error& e) {
                        std::cerr << "Error JSON type (" << text_to_evaluate << ", ID: " << identifier_for_text << "): "
                            << e.what() << ". Body: " << (res->body.size() > 80
                                ? res->body.substr(0, 80) + "..."
                                : res->body) << std::endl;
                    }
                } else {
                    std::cerr << "Error API (" << text_to_evaluate << ", ID: " << identifier_for_text << "): status " <<
                        res->status << ". Resp: " << (res->body.size() > 80
                            ? res->body.substr(0, 80) + "..."
                            : res->body) << std::endl;
                }
            } else {
                auto err_code = res.error();
                std::cerr << "Error HTTP lib (" << text_to_evaluate << ", ID: " << identifier_for_text << "): " <<
                    httplib::to_string(err_code) << (
                        err_code == httplib::Error::Connection && base_api_url_.rfind("http://localhost", 0) == 0
                        ? " (Verifique servidor en " + base_api_url_ + ")"
                        : "") << std::endl;
            }
        }

        std::sort(all_results.begin(), all_results.end());

        std::cout << "\nTop 3 Puntuaciones de Congruencia (API):" << std::endl;
        std::cout << std::fixed << std::setprecision(2);

        if (texts.empty()) {
            std::cout << "  No se proporcionaron textos." << std::endl;
        } else if (all_results.empty()) {
            std::cout << "  No se obtuvieron resultados válidos de la API." << std::endl;
        } else {
            int count = 0;
            for (const auto& result : all_results) {
                if (count < 3) {
                    std::cout << "  " << count + 1 << ". ID/Recorrido: [" << result.identifier
                        << "], Puntuación: " << result.congruence_score
                        << ", Texto: \"" << (
                            result.text.length() > 50 ? result.text.substr(0, 50) + "..." : result.text) << "\""
                        << std::endl;
                    count++;
                } else {
                    break;
                }
            }
            if (count > 0 && count < 3 && count == all_results.size()) {
                std::cout << "  (Se mostraron todos los " << count <<
                    " resultados disponibles ya que fueron menos de 3)" << std::endl;
            }
        }
    }

private:
    std::string base_api_url_;
    std::string api_path_;
    std::unique_ptr<httplib::Client> cli_;
};
