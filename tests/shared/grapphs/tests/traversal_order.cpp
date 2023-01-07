#include <grapphs/tests/traversal_order.h>

namespace gpp {

    expected_traversal_order::expected_traversal_order(
        const std::initializer_list<std::size_t>& elements
    ) : _allElements(elements), _available(_allElements) {

    }

    bool expected_traversal_order::pop(size_t current) {
        if (!_allElements.empty()) {
            auto numRemoved = _available.erase(current);
            if (numRemoved > 0) {
                return true;
            }
        }
        if (_next == nullptr) {
            return false;
        }
        return _next->pop(current);
    }

    std::shared_ptr<expected_traversal_order> expected_traversal_order::then(
        const std::initializer_list<std::size_t>& elements
    ) {
        _next = std::make_shared<expected_traversal_order>(elements);
        return _next;
    }

    const std::set<std::size_t>& expected_traversal_order::get_all_elements() const {
        return _allElements;
    }

    const std::set<std::size_t>& expected_traversal_order::get_available() const {
        return _available;
    }
}