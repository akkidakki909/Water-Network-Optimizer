#include "dinic.hpp"
#include "min_cost_flow.hpp"
#include "network_builder.hpp"
#include <iostream>
#include <vector>

int main() {
    using namespace water;
    const std::vector<Node> nodes = {
        {"ReservoirA", Node::Type::Source, 10},
        {"Junction1", Node::Type::Junction, 0},
        {"ZoneNorth", Node::Type::Sink, 8},
        {"ZoneSouth", Node::Type::Sink, 5}
    };
    const std::vector<Pipe> pipes = {
        {"ReservoirA", "Junction1", 9, 1, "Pipe-A"},
        {"Junction1", "ZoneNorth", 6, 2, "Pipe-B"},
        {"Junction1", "ZoneSouth", 5, 1, "Pipe-C"}
    };

    BuiltNetwork max_flow_network = build_network(nodes, pipes);
    long long max_flow = dinic_max_flow(max_flow_network.graph,
                                        max_flow_network.super_source,
                                        max_flow_network.super_sink);
    std::cout << "Water & Resource Distribution Network Optimizer\n";
    std::cout << "Algorithm: Dinic maximum flow\n";
    std::cout << "Maximum deliverable flow: " << max_flow << " units\n";
    print_pipe_results(max_flow_network);

    BuiltNetwork min_cost_network = build_network(nodes, pipes);
    auto [flow, cost] = min_cost_max_flow(min_cost_network.graph,
                                          min_cost_network.super_source,
                                          min_cost_network.super_sink);
    std::cout << "\nAlgorithm: Min-cost max-flow\n";
    std::cout << "Flow delivered: " << flow << " units\n";
    std::cout << "Total delivery cost: " << cost << "\n";
    print_pipe_results(min_cost_network);
    return 0;
}
