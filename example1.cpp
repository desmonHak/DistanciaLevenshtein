// main.cpp - Ejemplo COMPLETO usando Levenshtein.hpp
#include "Levenshtein.hpp"
#include <iostream>
#include <iomanip>
#include <vector>
#include <chrono>
#include <windows.h>

struct ConfigOption {
    std::string name;
    float affinity = 0.0f;

    const char* get_string() const { return name.c_str(); }
};

int main() {
    // Ejemplo SIMPLE: distancia directa
    std::string s1 = "identificar";
    std::string s2 = "identify";

    int dist = utils::Levenshtein::distance(s1, s2);
    float aff = utils::Levenshtein::affinity(s1, s2);

    std::cout << "Distancia: " << dist << std::endl;
    std::cout << "Afinidad: " << std::fixed << std::setprecision(2)
              << aff * 100 << "%" << std::endl << std::endl;

    // Array de configuraciones
    std::vector<ConfigOption> config_options = {
        {"config_path", 0},
        {"log_level", 0},
        {"max_connections", 0},
        {"timeout_duration", 0},
        {"enable_cache", 0},
        {"cache_size", 0},
        {"thread_count", 0},
        {"db_host", 0},
        {"db_port", 0},
        {"db_username", 0},
        {"project_name", 0},
        {"api_key", 0}
    };

    std::string search = "projet_name";  // Buscar

    // Medir rendimiento
    auto start = std::chrono::high_resolution_clock::now();

    // CALLBACK simple: actualizar affinity
    for (auto& option : config_options) {
        int dist = utils::Levenshtein::distance(search, option.name);
        option.affinity = utils::Levenshtein::affinity(search, option.name);

        // Top 3 resultados
        if (dist <= 2) {
            std::cout << "Dist: " << dist
                      << " Aff: " << std::fixed << std::setprecision(2)
                      << option.affinity * 100 << "% -> "
                      << option.name << std::endl;
        }
    }

    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);

    std::cout << "\nTiempo: " << duration.count() << " micro segundos" << std::endl;

    // find_closest() - Automático
    std::vector<std::string> candidates;
    for (const auto& opt : config_options) {
        candidates.push_back(opt.name);
    }

    std::string suggestion = utils::Levenshtein::find_closest(search, candidates);
    std::cout << "\nMejor sugerencia: '" << suggestion << "'" << std::endl;

    return 0;
}
