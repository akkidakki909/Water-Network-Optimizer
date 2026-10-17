#include "flow_network.hpp"
#include <stdexcept>

namespace water {

FlowNetwork::FlowNetwork(int vertex_count) {
    if (vertex_count < 0) throw std::invalid_argument("vertex_count cannot be negative");
    graph_.resize(static_cast<std::size_t>(vertex_count));
}

int FlowNetwork::vertex_count() const {
    return static_cast<int>(graph_.size());
}

int FlowNetwork::add_vertex() {
    graph_.emplace_back();
    return static_cast<int>(graph_.size()) - 1;
}

EdgeReference FlowNetwork::add_edge(int from, int to, long long capacity,
                                    long long cost, bool original_pipe,
                                    const std::string& pipe_name) {
    if (from < 0 || from >= vertex_count() || to < 0 || to >= vertex_count())
        throw std::out_of_range("edge endpoint out of range");
    if (capacity < 0) throw std::invalid_argument("capacity cannot be negative");

    const int forward_index = static_cast<int>(graph_[from].size());
    const int reverse_index = static_cast<int>(graph_[to].size());

    Edge forward{to, reverse_index, capacity, cost, capacity, original_pipe, pipe_name};
    Edge reverse{from, forward_index, 0, -cost, 0, false, ""};

    // Self-loops need special handling because both edges share one adjacency vector.
    if (from == to) {
        forward.reverse_index = forward_index + 1;
        reverse.reverse_index = forward_index;
        graph_[from].push_back(forward);
        graph_[from].push_back(reverse);
    } else {
        graph_[from].push_back(forward);
        graph_[to].push_back(reverse);
    }
    return {from, forward_index};
}

const std::vector<std::vector<Edge>>& FlowNetwork::adjacency() const { return graph_; }
std::vector<std::vector<Edge>>& FlowNetwork::adjacency() { return graph_; }

long long FlowNetwork::flow_on_edge(const EdgeReference& ref) const {
    const Edge& edge = graph_.at(static_cast<std::size_t>(ref.from)).at(static_cast<std::size_t>(ref.index));
    return edge.initial_capacity - edge.residual_capacity;
}

} // namespace water
