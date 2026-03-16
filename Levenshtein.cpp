// Levenshtein.cpp
#include "Levenshtein.hpp"

#include <functional>

extern "C" {
#include "Levenshtein_c.h"
}

namespace utils {
    int Levenshtein::distance(const std::string &str1, const std::string &str2,
                              bool               case_sensitive) {
        return ::Levenshtein(str1.c_str(), str2.c_str(), case_sensitive ? 1 : 0);
    }

    float Levenshtein::affinity(const std::string &str1, const std::string &str2) {
        size_t len1 = str1.length();
        size_t len2 = str2.length();
        int    dist = distance(str1, str2);
        return ::getAfinidad(len1, len2, dist);
    }

    std::string Levenshtein::find_closest(const std::string &             input,
                                          const std::vector<std::string> &candidates,
                                          int                             max_distance) {
        int         min_dist = max_distance + 1;
        std::string best;

        for (const auto &candidate: candidates) {
            int dist = distance(input, candidate);
            if (dist < min_dist) {
                min_dist = dist;
                best     = candidate;
            }
        }

        return (min_dist <= max_distance) ? best : std::string{};
    }

    template<typename T>
    void Levenshtein::find_in_array(const std::string &                                 input,
                                    const std::vector<T *> &                            data,
                                    std::function<const char*(const T *)>               get_string,
                                    std::function<void(int dist, float aff, const T *)> callback) {
        for (T *element: data) {
            const char *str  = get_string(element);
            int         dist = distance(input, str);
            float       aff  = affinity(input, str);
            callback(dist, aff, element);
        }
    }
} // namespace vm::utils
