#ifndef LEVENSHTEIN_HPP
#define LEVENSHTEIN_HPP
#include <functional>
#include <string>

namespace utils {
    class Levenshtein {
    public:
        /**
         * @brief Distancia de Levenshtein optimizada O(n) espacio.
         * @param str1 Cadena 1
         * @param str2 Cadena 2
         * @param case_sensitive Case-sensitive (default: false)
         * @return Número de operaciones (inserción/eliminación/sustitución)
         */
        static int distance(const std::string &str1, const std::string &str2,
                            bool               case_sensitive = false);

        /**
         * @brief Afinidad = 1 - (distancia / longitud_máxima)
         * @param str1 Cadena 1
         * @param str2 Cadena 2
         * @return Float [0.0, 1.0] - 1.0 = idénticas
         */
        static float affinity(const std::string &str1, const std::string &str2);

        /**
         * @brief Buscar mejor coincidencia en vector<string>
         * @param input Cadena a comparar
         * @param candidates Lista de opciones
         * @param max_distance Máx distancia aceptable (default: 2)
         * @return Mejor coincidencia o string vacío
         */
        static std::string find_closest(const std::string &             input,
                                        const std::vector<std::string> &candidates,
                                        int                             max_distance = 2);

        /**
         * @brief Buscar en array custom con callbacks
         * @param input Cadena a buscar
         * @param data Array opaco
         * @param size Tamaño array
         * @param get_string Callback: string(element)
         * @param callback Callback: (dist, affinity, element)
         */
        template<typename T>
        static void find_in_array(const std::string &                                 input,
                                  const std::vector<T *> &                            data,
                                  std::function<const char*(const T *)>               get_string,
                                  std::function<void(int dist, float aff, const T *)> callback);
    };
} // namespace vm::utils

#endif //LEVENSHTEIN_HPP
