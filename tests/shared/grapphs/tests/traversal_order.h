#ifndef UNNECESSARYENGINE_TRAVERSAL_ORDER_H
#define UNNECESSARYENGINE_TRAVERSAL_ORDER_H

#include <set>
#include <memory>
#include <grapphs/graph.h>
#include <sstream>

namespace gpp {
    class ExpectedTraversalOrder {
    private:
        std::set<std::size_t> allElements;
        std::set<std::size_t> available;
        std::shared_ptr<ExpectedTraversalOrder> next;
    public:
        ExpectedTraversalOrder(
            const std::initializer_list<std::size_t>& elements
        );

        std::shared_ptr<ExpectedTraversalOrder> then(const std::initializer_list<std::size_t>& elements);

        bool pop(size_t current);

        const std::set<std::size_t>& getAllElements() const;

        const std::set<std::size_t>& getAvailable() const;

        std::string listAvailable() const {
            std::stringstream ss;
            ss << "Available: ";
            const ExpectedTraversalOrder* current = this;
            while (current != nullptr) {
                for (const auto& item : current->available) {
                    ss << item << ", ";
                }
                current = current->next.get();
            }
            return ss.str();
        }
    };
}
#endif
