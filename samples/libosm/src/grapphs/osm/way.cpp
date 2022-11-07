#include <grapphs/osm/way.h>

namespace gpp::osm {

    Coordinate::Coordinate(double longitude, double latitude)
        : _longitude(longitude), _latitude(latitude) {
    }

    double Coordinate::get_longitude() const {
        return _longitude;
    }

    double Coordinate::get_latitude() const {
        return _latitude;
    }

    std::ostream& operator<<(std::ostream& os, const Coordinate& coordinate) {
        return os << "(longitude: " << coordinate._longitude << ", latitude: "
                  << coordinate._latitude << ")";
    }

    const Coordinate& Node::get_location() const {
        return _location;
    }

    Node::Node(const Coordinate& location) : _location(location) {
    }

    std::ostream& operator<<(std::ostream& os, const Node& node) {
        os << "_location: " << node._location;
        return os;
    }

    Way::Way(size_t metadataIndex) : _metadataIndex(metadataIndex) {
    }

    size_t Way::get_metadata_index() const {
        return _metadataIndex;
    }

    std::ostream& operator<<(std::ostream& os, const Way& way) {
        os << "_metadataIndex: " << way._metadataIndex;
        return os;
    }

    WayMetadata::WayMetadata(
        const std::string& name, WayMetadata::Flags flags, WayMetadata::Kind kind
    ) : _name(name), _flags(flags), _kind(kind) {
    }

    std::size_t OSMGraph::push_meta(WayMetadata&& metadata) {
        std::size_t i = _metadata.size();
        _metadata.push_back(std::move(metadata));
        return i;
    }

    bool OSMGraph::get_metadata(const Way& way, WayMetadata& metadata) {
        std::size_t index = way.get_metadata_index();
        if (index >= _metadata.size()) {
            return false;
        }
        metadata = _metadata[index];
        return true;
    }
}