#ifndef GRAPPHS_PARSE_H
#define GRAPPHS_PARSE_H

#include <grapphs/osm/way.h>

#include <filesystem>

namespace gpp::osm {

    void parse(const std::filesystem::path& file, gpp::osm::OSMGraph& into);
}

#endif
