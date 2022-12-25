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

    class WayMetadata {
    public:
        WayMetadata(const WayMetadata&) = default;

        enum Flags {
            eSidewalkLeft = 1 << 0,
            eSidewalkRight = 1 << 1,
            eSidewalkBoth = eSidewalkLeft | eSidewalkRight,
        };
        enum Kind : uint8_t {
            eDirt, eRoad, eAvenue, eHighway, eUnknown
        };

    private:
        std::string _name;
        Flags _flags;
        Kind _kind;

    public:
        WayMetadata(const std::string& name, Flags flags, Kind kind);

        const std::string& get_name() const;

        Flags get_flags() const;

        Kind get_kind() const;
    };

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

    class OSMGraph : public gpp::AdjacencyList<Node, Way> {
    private:
        std::vector<WayMetadata> _metadata;

    public:
        OSMGraph() = default;

        std::size_t push_meta(gpp::osm::WayMetadata&& metadata);

        bool get_metadata(const Way& way, gpp::osm::WayMetadata& metadata);
    };
} // namespace gpp::osm
#endif
