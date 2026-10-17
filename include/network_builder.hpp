#pragma once
#include "flow_network.hpp"
#include <string>
#include <vector>

namespace water {

struct Node {
    std::string id;
    enum class Type { Source, Junction, Sink } type;
    long long amount = 0; // source supply or sink demand limit
};

struct Pipe {
    std::string from;
    std::string to;
    long long capacity;
    long long cost = 0;
    std::string id;
};

struct BuiltNetwork {
    FlowNetwork graph;
    int super_source;
    int super_sink;
    std::vector<std::string> node_ids;
    std::vector<EdgeReference> pipe_edges;
    std::vector<std::string> pipe_ids;
    long long total_demand = 0;
};

BuiltNetwork build_network(const std::vector<Node>& nodes, const std::vector<Pipe>& pipes);
void print_pipe_results(const BuiltNetwork& built);

} // namespace water
