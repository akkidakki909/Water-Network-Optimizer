#include "dinic.hpp"
#include "min_cost_flow.hpp"
#include "network_builder.hpp"
#include <cassert>
#include <iostream>
#include <vector>

using namespace water;

static void test_clrs_max_flow() {
    FlowNetwork g(6);
    g.add_edge(0, 1, 16); g.add_edge(0, 2, 13);
    g.add_edge(1, 2, 10); g.add_edge(2, 1, 4);
    g.add_edge(1, 3, 12); g.add_edge(3, 2, 9);
    g.add_edge(2, 4, 14); g.add_edge(4, 3, 7);
    g.add_edge(3, 5, 20); g.add_edge(4, 5, 4);
    assert(dinic_max_flow(g, 0, 5) == 23);
}

static void test_no_path() {
    FlowNetwork g(3);
    g.add_edge(0, 1, 7);
    assert(dinic_max_flow(g, 0, 2) == 0);
}

static void test_min_cost_flow() {
    FlowNetwork g(4);
    g.add_edge(0, 1, 2, 1);
    g.add_edge(0, 2, 1, 0);
    g.add_edge(1, 2, 1, 0);
    g.add_edge(1, 3, 1, 2);
    g.add_edge(2, 3, 2, 3);
    auto result = min_cost_max_flow(g, 0, 3);
    assert(result.first == 3);
    assert(result.second == 10);
}

static void test_multi_source_sink_builder() {
    std::vector<Node> nodes = {
        {"S1", Node::Type::Source, 4},
        {"J", Node::Type::Junction, 0},
        {"T1", Node::Type::Sink, 3},
        {"T2", Node::Type::Sink, 2}
    };
    std::vector<Pipe> pipes = {
        {"S1", "J", 4, 1, "P1"},
        {"J", "T1", 3, 2, "P2"},
        {"J", "T2", 2, 1, "P3"}
    };
    auto built = build_network(nodes, pipes);
    assert(dinic_max_flow(built.graph, built.super_source, built.super_sink) == 4);
    assert(built.total_demand == 5);
}

int main() {
    test_clrs_max_flow();
    test_no_path();
    test_min_cost_flow();
    test_multi_source_sink_builder();
    std::cout << "All 4 C++ tests passed.\n";
    return 0;
}
