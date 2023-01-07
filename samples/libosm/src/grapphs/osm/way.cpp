#include <grapphs/osm/way.h>

namespace gpp::osm {

    coordinate::coordinate(double longitude, double latitude)
        : _longitude(longitude), _latitude(latitude) {
    }

    double coordinate::get_longitude() const {
        return _longitude;
    }

    double coordinate::get_latitude() const {
        return _latitude;
    }

    std::ostream& operator<<(std::ostream& os, const coordinate& coordinate) {
        return os << "(longitude: " << coordinate._longitude << ", latitude: "
                  << coordinate._latitude << ")";
    }

    const coordinate& osm_node::get_location() const {
        return _location;
    }

    osm_node::osm_node(const coordinate& location) : _location(location) {
    }

    std::ostream& operator<<(std::ostream& os, const osm_node& node) {
        os << "_location: " << node._location;
        return os;
    }

    way::way(size_t metadataIndex) : _metadataIndex(metadataIndex) {
    }

    size_t way::get_metadata_index() const {
        return _metadataIndex;
    }

    std::ostream& operator<<(std::ostream& os, const way& way) {
        os << "_metadataIndex: " << way._metadataIndex;
        return os;
    }

    way_metadata::flags operator|=(way_metadata::flags a, way_metadata::flags b) {
        return a | b;
    }

    way_metadata::flags operator|(way_metadata::flags a, way_metadata::flags b) {
        return static_cast<way_metadata::flags>(static_cast<uint8_t>(a) | static_cast<uint8_t>(b));
    }
    way_metadata::flags operator&(way_metadata::flags a, way_metadata::flags b) {
        return static_cast<way_metadata::flags>(static_cast<uint8_t>(a) & static_cast<uint8_t>(b));
    }

    bool way::has_metadata() const {
        return _metadataIndex != invalid_metadata();
    }

    const std::string& way_metadata::get_name() const {
        return _name;
    }

    way_metadata::flags way_metadata::get_flags() const {
        return _flags;
    }

    way_metadata::surface way_metadata::get_surface() const {
        return _surface;
    }

    way_metadata::way_metadata(
        const std::string& name,
        float maxSpeed,
        way_metadata::flags flags,
        way_metadata::kind kind,
        way_metadata::surface surface
    ) : _name(name), _maxSpeed(maxSpeed), _flags(flags), _kind(kind), _surface(surface) {
    }

    float way_metadata::get_max_speed() const {
        return _maxSpeed;
    }

    way_metadata::kind way_metadata::get_kind() const {
        return _kind;
    }

    std::size_t osm_graph::push_meta(way_metadata&& metadata) {
        std::size_t i = _metadata.size();
        _metadata.push_back(std::move(metadata));
        return i;
    }

    bool osm_graph::get_metadata(const way& way, way_metadata& metadata) {
        std::size_t index = way.get_metadata_index();
        if (index >= _metadata.size()) {
            return false;
        }
        metadata = _metadata[index];
        return true;
    }
}