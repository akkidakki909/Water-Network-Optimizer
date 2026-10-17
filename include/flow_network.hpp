#pragma once
#include <vector>
#include <string>
#include <cstddef>

namespace water {

struct Edge {
    int to;
    int reverse_index;
    long long residual_capacity;
    long long cost;
    long long initial_capacity;
    bool original_pipe;
    std::string pipe_name;
};

struct EdgeReference {
    int from;
    int index;
};

class FlowNetwork {
public:
    explicit FlowNetwork(int vertex_count = 0);

    int vertex_count() const;
    int add_vertex();
    EdgeReference add_edge(int from, int to, long long capacity,
                           long long cost = 0, bool original_pipe = false,
                           const std::string& pipe_name = "");
    const std::vector<std::vector<Edge>>& adjacency() const;
    std::vector<std::vector<Edge>>& adjacency();
    long long flow_on_edge(const EdgeReference& ref) const;

private:
    std::vector<std::vector<Edge>> graph_;
};

} // namespace water
