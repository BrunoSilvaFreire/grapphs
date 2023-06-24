#ifndef GRAPPHS_STRING_HELPER_H
#define GRAPPHS_STRING_HELPER_H
#include <string>
#include <sstream>

namespace gpp::tests {
    template<typename t_container>
    std::string join_string(const t_container& container) {
        std::stringstream ss;
        using iterator = typename t_container::iterator;
        iterator antepenultimate = --container.end();
        for (iterator ptr = container.begin(); ptr != container.end(); ptr++) {
            bool isLast = ptr == antepenultimate;

            const auto& element = *ptr;
            ss << element;

            if (!isLast) {
                ss << ", ";
            }
        }
        return ss.str();
    }
}
#endif