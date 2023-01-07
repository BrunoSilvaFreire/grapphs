#ifndef GRAPPHS_WAY_H
#define GRAPPHS_WAY_H

#include <grapphs/adjacency_list.h>

#include <ostream>
#include <string>
#include <vector>

namespace gpp::osm {

    struct coordinate {
    private:
        double _longitude, _latitude;

    public:
        coordinate(double longitude, double latitude);

        double get_longitude() const;

        double get_latitude() const;

        friend std::ostream& operator<<(std::ostream& os, const coordinate& coordinate);
    };

    class osm_node {
    private:
        coordinate _location;

    public:
        const coordinate& get_location() const;

        explicit osm_node(const coordinate& location);

        friend std::ostream& operator<<(std::ostream& os, const osm_node& node);
    };

    class lane {
    };

    class way_metadata {
    public:
        way_metadata(const way_metadata&) = default;

        enum class flags : uint8_t {
            SIDEWALK_LEFT = 1 << 0,
            SIDEWALK_RIGHT = 1 << 1,
            LIT = 1 << 2,
            BUILDING,
            SIDEWALK_BOTH = SIDEWALK_LEFT | SIDEWALK_RIGHT
        };

        enum class surface : uint8_t {
            DIRT, ASPHALT, UNKNOWN
        };
        enum class kind : uint8_t {
            WAY, ROAD, AVENUE, HIGHWAY, UNKNOWN
        };

    private:
        std::string _name;
        float _maxSpeed;
        flags _flags;
        kind _kind;
        surface _surface;

    public:
        way_metadata() = default;

        way_metadata(const std::string& name, float maxSpeed, flags flags, kind kind, surface surface);

        const std::string& get_name() const;

        flags get_flags() const;

        surface get_surface() const;

        float get_max_speed() const;

        kind get_kind() const;
    };

    way_metadata::flags operator|(way_metadata::flags a, way_metadata::flags b);

    way_metadata::flags operator&(way_metadata::flags a, way_metadata::flags b);

    way_metadata::flags operator|=(way_metadata::flags a, way_metadata::flags b);

    class way {
    private:
        std::size_t _metadataIndex;

    public:
        static constexpr std::size_t invalid_metadata() {
            return -1;
        }

        way(size_t metadataIndex = invalid_metadata());

        size_t get_metadata_index() const;

        friend std::ostream& operator<<(std::ostream& os, const way& way);

        bool has_metadata() const;
    };

    class osm_graph : public gpp::adjacency_list<osm_node, way> {
    private:
        std::vector<way_metadata> _metadata;

    public:
        osm_graph() = default;

        std::size_t push_meta(gpp::osm::way_metadata&& metadata);

        bool get_metadata(const way& way, gpp::osm::way_metadata& metadata);
    };
} // namespace gpp::osm
#endif
