#ifndef GRAPPHS_WAY_H
#define GRAPPHS_WAY_H

#include <grapphs/adjacency_list.h>

#include <ostream>
#include <string>
#include <vector>

namespace gpp::osm {

    struct Coordinate {
    private:
        double _longitude, _latitude;

    public:
        Coordinate(double longitude, double latitude);

        double get_longitude() const;

        double get_latitude() const;

        friend std::ostream& operator<<(std::ostream& os, const Coordinate& coordinate);
    };

    class Node {
    private:
        Coordinate _location;

    public:
        const Coordinate& get_location() const;

        explicit Node(const Coordinate& location);

        friend std::ostream& operator<<(std::ostream& os, const Node& node);
    };

    class Lane {
    };

    class way_metadata {
    public:
        way_metadata(const way_metadata&) = default;

        enum class Flags : uint8_t {
            eSidewalkLeft = 1 << 0,
            eSidewalkRight = 1 << 1,
            eLit = 1 << 2,
            eBuilding,
            eSidewalkBoth = eSidewalkLeft | eSidewalkRight
        };

        enum class Surface : uint8_t {
            eDirt, eAsphalt, eUnknown
        };
        enum class Kind : uint8_t {
            eWay, eRoad, eAvenue, eHighway, eUnknown
        };

    private:
        std::string _name;
        float _maxSpeed;
        Flags _flags;
        Kind _kind;
        Surface _surface;

    public:
        way_metadata() = default;

        way_metadata(const std::string& name, float maxSpeed, Flags flags, Kind kind, Surface surface);

        const std::string& get_name() const;

        Flags get_flags() const;

        Surface get_surface() const;

        float get_max_speed() const;

        Kind get_kind() const;
    };

    way_metadata::Flags operator|(way_metadata::Flags a, way_metadata::Flags b);

    way_metadata::Flags operator&(way_metadata::Flags a, way_metadata::Flags b);

    way_metadata::Flags operator|=(way_metadata::Flags a, way_metadata::Flags b);

    class Way {
    private:
        std::size_t _metadataIndex;

    public:
        static constexpr std::size_t invalid_metadata() {
            return -1;
        }

        Way(size_t metadataIndex = invalid_metadata());

        size_t get_metadata_index() const;

        friend std::ostream& operator<<(std::ostream& os, const Way& way);

        bool has_metadata() const;
    };

    class OSMGraph : public gpp::adjacency_list<Node, Way> {
    private:
        std::vector<way_metadata> _metadata;

    public:
        OSMGraph() = default;

        std::size_t push_meta(gpp::osm::way_metadata&& metadata);

        bool get_metadata(const Way& way, gpp::osm::way_metadata& metadata);
    };
} // namespace gpp::osm
#endif
