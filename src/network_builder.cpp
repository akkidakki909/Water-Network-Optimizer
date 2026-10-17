#include "network_builder.hpp"
#include <iostream>
#include <stdexcept>
#include <unordered_map>

namespace water {
BuiltNetwork build_network(const std::vector<Node>& nodes, const std::vector<Pipe>& pipes) {
    std::unordered_map<std::string, int> index;
    for (int i = 0; i < static_cast<int>(nodes.size()); ++i) {
        if (nodes[i].id.empty()) throw std::invalid_argument("node id cannot be empty");
        if (!index.emplace(nodes[i].id, i).second)
            throw std::invalid_argument("duplicate node id: " + nodes[i].id);
        if (nodes[i].amount < 0) throw std::invalid_argument("node amount cannot be negative");
    }

    int super_source = static_cast<int>(nodes.size());
    int super_sink = super_source + 1;
    BuiltNetwork built{FlowNetwork(static_cast<int>(nodes.size()) + 2),
                       super_source, super_sink, {}, {}, {}, 0};

    for (const Node& node : nodes) built.node_ids.push_back(node.id);

    for (const Pipe& pipe : pipes) {
        auto from = index.find(pipe.from);
        auto to = index.find(pipe.to);
        if (from == index.end() || to == index.end())
            throw std::invalid_argument("pipe references an unknown node: " + pipe.id);
        if (pipe.capacity < 0) throw std::invalid_argument("pipe capacity cannot be negative");
        std::string pipe_name = pipe.id.empty() ? pipe.from + "->" + pipe.to : pipe.id;
        built.pipe_edges.push_back(built.graph.add_edge(
            from->second, to->second, pipe.capacity, pipe.cost, true, pipe_name));
        built.pipe_ids.push_back(pipe_name);
    }

    for (int i = 0; i < static_cast<int>(nodes.size()); ++i) {
        if (nodes[i].type == Node::Type::Source && nodes[i].amount > 0) {
            built.graph.add_edge(super_source, i, nodes[i].amount);
        } else if (nodes[i].type == Node::Type::Sink && nodes[i].amount > 0) {
            built.graph.add_edge(i, super_sink, nodes[i].amount);
            built.total_demand += nodes[i].amount;
        }
    }
    return built;
}

void print_pipe_results(const BuiltNetwork& built) {
    std::cout << "\nPipe utilization:\n";
    for (std::size_t i = 0; i < built.pipe_edges.size(); ++i) {
        std::cout << "  " << built.pipe_ids[i] << ": "
                  << built.graph.flow_on_edge(built.pipe_edges[i]) << " units used\n";
    }
}
} // namespace water
