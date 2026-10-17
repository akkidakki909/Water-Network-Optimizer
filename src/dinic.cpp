#include "dinic.hpp"
#include <algorithm>
#include <queue>
#include <stdexcept>
#include <limits>

namespace water {
namespace {
bool build_levels(const FlowNetwork& network, int source, int sink, std::vector<int>& level) {
    const auto& graph = network.adjacency();
    std::fill(level.begin(), level.end(), -1);
    std::queue<int> q;
    level[source] = 0;
    q.push(source);

    while (!q.empty()) {
        int v = q.front(); q.pop();
        for (const Edge& edge : graph[v]) {
            if (edge.residual_capacity > 0 && level[edge.to] == -1) {
                level[edge.to] = level[v] + 1;
                q.push(edge.to);
            }
        }
    }
    return level[sink] != -1;
}

long long send_flow(FlowNetwork& network, int v, int sink, long long pushed,
                    const std::vector<int>& level, std::vector<std::size_t>& next_edge) {
    if (pushed == 0) return 0;
    if (v == sink) return pushed;

    auto& graph = network.adjacency();
    for (std::size_t& i = next_edge[v]; i < graph[v].size(); ++i) {
        Edge& edge = graph[v][i];
        if (edge.residual_capacity <= 0 || level[edge.to] != level[v] + 1) continue;
        long long sent = send_flow(network, edge.to, sink,
                                   std::min(pushed, edge.residual_capacity), level, next_edge);
        if (sent > 0) {
            edge.residual_capacity -= sent;
            graph[edge.to][edge.reverse_index].residual_capacity += sent;
            return sent;
        }
    }
    return 0;
}
}

long long dinic_max_flow(FlowNetwork& network, int source, int sink) {
    const int n = network.vertex_count();
    if (source < 0 || source >= n || sink < 0 || sink >= n)
        throw std::out_of_range("source or sink out of range");
    if (source == sink) return 0;

    long long total_flow = 0;
    std::vector<int> level(static_cast<std::size_t>(n));
    while (build_levels(network, source, sink, level)) {
        std::vector<std::size_t> next_edge(static_cast<std::size_t>(n), 0);
        while (true) {
            long long sent = send_flow(network, source, sink,
                                       std::numeric_limits<long long>::max(),
                                       level, next_edge);
            if (sent == 0) break;
            total_flow += sent;
        }
    }
    return total_flow;
}
} // namespace water
