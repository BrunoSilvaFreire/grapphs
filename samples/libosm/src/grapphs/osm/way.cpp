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

    WayMetadata::Flags operator|=(WayMetadata::Flags a, WayMetadata::Flags b) {
        return a | b;
    }

    WayMetadata::Flags operator|(WayMetadata::Flags a, WayMetadata::Flags b) {
        return static_cast<WayMetadata::Flags>(static_cast<uint8_t>(a) | static_cast<uint8_t>(b));
    }
    WayMetadata::Flags operator&(WayMetadata::Flags a, WayMetadata::Flags b) {
        return static_cast<WayMetadata::Flags>(static_cast<uint8_t>(a) & static_cast<uint8_t>(b));
    }

    bool Way::has_metadata() const {
        return _metadataIndex != invalid_metadata();
    }


    const std::string& WayMetadata::get_name() const {
        return _name;
    }

    WayMetadata::Flags WayMetadata::get_flags() const {
        return _flags;
    }

    WayMetadata::Surface WayMetadata::get_surface() const {
        return _surface;
    }

    WayMetadata::WayMetadata(
        const std::string& name,
        float maxSpeed,
        WayMetadata::Flags flags,
        WayMetadata::Kind kind,
        WayMetadata::Surface surface
    ) : _name(name), _maxSpeed(maxSpeed), _flags(flags), _kind(kind), _surface(surface) {
    }

    float WayMetadata::get_max_speed() const {
        return _maxSpeed;
    }

    WayMetadata::Kind WayMetadata::get_kind() const {
        return _kind;
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