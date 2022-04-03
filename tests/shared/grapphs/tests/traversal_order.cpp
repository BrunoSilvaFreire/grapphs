#include <grapphs/tests/traversal_order.h>

namespace gpp {

    ExpectedTraversalOrder::ExpectedTraversalOrder(
        const std::initializer_list<std::size_t>& elements
    ) : allElements(elements), available(allElements) {

    }

    bool ExpectedTraversalOrder::pop(size_t current) {
        if (!allElements.empty()) {
            auto numRemoved = available.erase(current);
            if (numRemoved > 0) {
                return true;
            }
        }
        if (next == nullptr) {
            return false;
        }
        return next->pop(current);
    }

    std::shared_ptr<ExpectedTraversalOrder> ExpectedTraversalOrder::then(
        const std::initializer_list<std::size_t>& elements
    ) {
        next = std::make_shared<ExpectedTraversalOrder>(elements);
        return next;
    }

    const std::set<std::size_t>& ExpectedTraversalOrder::getAllElements() const {
        return allElements;
    }

    const std::set<std::size_t>& ExpectedTraversalOrder::getAvailable() const {
        return available;
    }
}