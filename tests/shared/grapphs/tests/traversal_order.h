#ifndef GRAPPHS_TRAVERSAL_ORDER_H
#define GRAPPHS_TRAVERSAL_ORDER_H

#include <set>
#include <memory>
#include <grapphs/graph.h>
#include <sstream>

namespace gpp {
    class expected_traversal_order {
    private:
        std::set<std::size_t> _allElements;
        std::set<std::size_t> _available;
        std::shared_ptr<expected_traversal_order> _next;
    public:
        expected_traversal_order(
            const std::initializer_list<std::size_t>& elements
        );

        std::shared_ptr<expected_traversal_order>
        then(const std::initializer_list<std::size_t>& elements);

        bool pop(size_t current);

        const std::set<std::size_t>& get_all_elements() const;

        const std::set<std::size_t>& get_available() const;

        std::string list_available() const {
            std::stringstream ss;
            ss << "Available: ";
            const expected_traversal_order* current = this;
            while (current != nullptr) {
                for (const auto& item : current->_available) {
                    ss << item << ", ";
                }
                current = current->_next.get();
            }
            return ss.str();
        }
    };
}
#endif
