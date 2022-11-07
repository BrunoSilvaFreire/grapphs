#include <grapphs/osm/parse.h>
#include <readosm.h>

#include <iostream>
#include <unordered_map>

namespace gpp::osm {
    class ParserHelper {
    private:
        std::unordered_map<std::size_t, std::size_t> _osm2gpp;
        gpp::osm::OSMGraph* _graph;

    public:
        explicit ParserHelper(OSMGraph* graph) : _graph(graph), _osm2gpp() {
        }

        void add(std::size_t osmIndex, gpp::osm::Node&& node) {
            std::size_t gpp = _graph->push(std::forward<gpp::osm::Node&&>(node));
            _osm2gpp[osmIndex] = gpp;
        }

        gpp::osm::OSMGraph& get_graph() {
            return *_graph;
        }

        std::size_t get_gpp_index(std::size_t of) const {
            return _osm2gpp.at(of);
        }
    };

    int node_parse(const void* pHelper, const readosm_node* node) {
        gpp::osm::Coordinate loc(node->longitude, node->latitude);
        auto& into = const_cast<ParserHelper&>(*static_cast<const ParserHelper*>(pHelper));

        gpp::osm::Node gppNode(loc);

        into.add(node->id, std::move(gppNode));
        return READOSM_OK;
    }

    std::string find_tag(const std::string& key, const readosm_way* way) {
        for (int i = 0; i < way->tag_count; ++i) {
            const auto& tag = way->tags[i];
            if (key == tag.key) {
                return tag.value;
            }
        }
        return "";
    }

    int way_parse(const void* pHelper, const readosm_way* way) {
        auto& into = const_cast<ParserHelper&>(*static_cast<const ParserHelper*>(pHelper));

        /*std::cout << "Parsed way @ " << way->id << " (" << way->node_ref_count << " node refs): "
                  << std::endl;

        for (int i = 0; i < way->node_ref_count; ++i) {
            std::size_t nodeID = way->node_refs[i];
            std::cout << "#" << i << ": " << nodeID << std::endl;
        }

        if (way->node_ref_count > 2) {
            std::cout << "Interesting way tags (" << way->tag_count << "):" << std::endl;

            for (std::size_t i = 0; i < way->tag_count; ++i) {
                const readosm_tag& tag = way->tags[i];
                std::cout << "Tag #" << i << ": " << tag.key << " -> " << tag.value << std::endl;
            }
        }*/
        std::string name = find_tag("name", way);

        gpp::osm::WayMetadata::Flags flags{};
        gpp::osm::WayMetadata::Kind kind = gpp::osm::WayMetadata::Kind::eUnknown;

        gpp::osm::WayMetadata meta(name, flags, kind);

        OSMGraph& graph = into.get_graph();

        std::size_t metaIndex = graph.push_meta(std::move(meta));

        for (int i = 1; i < way->node_ref_count; ++i) {
            const auto& from = into.get_gpp_index(way->node_refs[i - 1]);
            const auto& to = into.get_gpp_index(way->node_refs[i]);
            graph.connect(from, to, gpp::osm::Way(metaIndex));
        }

        std::cout << std::endl;

        return READOSM_OK;
    }

    int relation_parse(const void* pHelper, const readosm_relation* relation) {
      /*  std::cout << "Parsed relation @ " << relation->id << " (" << relation->member_count
                  << " members)." << std::endl;
        for (std::size_t i = 0; i < relation->member_count; ++i) {
            const readosm_member& member = relation->members[i];
            std::cout << "#" << i << ": " << member.id << ", type: " << member.member_type
                      << ", role " << member.role << std::endl;
        }*/

        return READOSM_OK;
    }

    void parse(const std::filesystem::path& file, gpp::osm::OSMGraph& into) {
        const void* osmHandle;
        ParserHelper helper(&into);
        readosm_open(file.c_str(), reinterpret_cast<const void**>(&osmHandle));

        readosm_node_callback pfnNodeCB = &node_parse;
        readosm_way_callback pfnWayCB = &way_parse;
        readosm_relation_callback pfnRelationCB = &relation_parse;

        readosm_parse(osmHandle, &helper, pfnNodeCB, pfnWayCB, pfnRelationCB);
        readosm_close(osmHandle);
    }
}  // namespace gpp::osm