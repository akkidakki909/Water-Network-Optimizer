#include "dinic.hpp"
#include "demand_forecast.hpp"
#include "min_cost_flow.hpp"
#include "network_builder.hpp"
#include <iostream>
#include <vector>
int main() {
    using namespace water;
    const auto north = forecast_demand({{1,4},{2,5},{3,6},{4,7},{5,8}}, 6);
    const auto south = forecast_demand({{1,3},{2,3},{3,4},{4,4},{5,5}}, 6);
    std::cout << "Water & Resource Distribution Network Optimizer - Week 2\n";
    std::cout << "Demand forecast (linear-regression baseline):\n";
    std::cout << "  ZoneNorth predicted: " << north.predicted_demand << " (demand limit " << north.recommended_demand_limit << ")\n";
    std::cout << "  ZoneSouth predicted: " << south.predicted_demand << " (demand limit " << south.recommended_demand_limit << ")\n";
    const std::vector<Node> nodes = {
        {"ReservoirA", Node::Type::Source, 15}, {"Junction1", Node::Type::Junction, 0},
        {"ZoneNorth", Node::Type::Sink, north.recommended_demand_limit},
        {"ZoneSouth", Node::Type::Sink, south.recommended_demand_limit}
    };
    const std::vector<Pipe> pipes = {
        {"ReservoirA", "Junction1", 13, 1, "Pipe-A"},
        {"Junction1", "ZoneNorth", 9, 2, "Pipe-B"},
        {"Junction1", "ZoneSouth", 7, 1, "Pipe-C"}
    };
    auto max_network = build_network(nodes, pipes);
    const auto max_flow = dinic_max_flow(max_network.graph, max_network.super_source, max_network.super_sink);
    std::cout << "\nDinic maximum deliverable flow: " << max_flow << " units\n";
    print_pipe_results(max_network);
    auto cost_network = build_network(nodes, pipes);
    const auto [flow, cost] = min_cost_max_flow(cost_network.graph, cost_network.super_source, cost_network.super_sink);
    std::cout << "\nMin-cost max-flow delivered: " << flow << " units; total cost: " << cost << "\n";
    print_pipe_results(cost_network);
}
