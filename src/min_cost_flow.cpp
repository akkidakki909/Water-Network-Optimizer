#include "min_cost_flow.hpp"
#include <algorithm>
#include <functional>
#include <limits>
#include <queue>
#include <stdexcept>
#include <vector>

namespace water {
std::pair<long long, long long> min_cost_max_flow(
    FlowNetwork& network, int source, int sink) {
    const int n = network.vertex_count();
    if (source < 0 || source >= n || sink < 0 || sink >= n)
        throw std::out_of_range("source or sink out of range");
    if (source == sink) return {0, 0};

    const long long INF = std::numeric_limits<long long>::max() / 4;
    auto& graph = network.adjacency();
    std::vector<long long> potential(static_cast<std::size_t>(n), 0);

    // Bellman-Ford computes feasible initial potentials when negative costs exist.
    // Assumes there is no reachable negative-cost cycle in the initial residual graph.
    std::vector<long long> dist(static_cast<std::size_t>(n), INF);
    dist[source] = 0;
    for (int iteration = 0; iteration < n - 1; ++iteration) {
        bool changed = false;
        for (int u = 0; u < n; ++u) {
            if (dist[u] == INF) continue;
            for (const Edge& edge : graph[u]) {
                if (edge.residual_capacity > 0 && dist[edge.to] > dist[u] + edge.cost) {
                    dist[edge.to] = dist[u] + edge.cost;
                    changed = true;
                }
            }
        }
        if (!changed) break;
    }
    for (int v = 0; v < n; ++v) if (dist[v] != INF) potential[v] = dist[v];

    long long total_flow = 0;
    long long total_cost = 0;

    while (true) {
        std::fill(dist.begin(), dist.end(), INF);
        std::vector<int> previous_vertex(static_cast<std::size_t>(n), -1);
        std::vector<int> previous_edge(static_cast<std::size_t>(n), -1);
        using QueueItem = std::pair<long long, int>;
        std::priority_queue<QueueItem, std::vector<QueueItem>, std::greater<QueueItem>> pq;
        dist[source] = 0;
        pq.push({0, source});

        while (!pq.empty()) {
            auto [current_dist, u] = pq.top(); pq.pop();
            if (current_dist != dist[u]) continue;
            for (int i = 0; i < static_cast<int>(graph[u].size()); ++i) {
                const Edge& edge = graph[u][i];
                if (edge.residual_capacity <= 0) continue;
                long long reduced_cost = edge.cost + potential[u] - potential[edge.to];
                if (dist[edge.to] > current_dist + reduced_cost) {
                    dist[edge.to] = current_dist + reduced_cost;
                    previous_vertex[edge.to] = u;
                    previous_edge[edge.to] = i;
                    pq.push({dist[edge.to], edge.to});
                }
            }
        }
        if (dist[sink] == INF) break;

        for (int v = 0; v < n; ++v) if (dist[v] != INF) potential[v] += dist[v];

        long long add_flow = INF;
        for (int v = sink; v != source; v = previous_vertex[v]) {
            int u = previous_vertex[v];
            if (u == -1) { add_flow = 0; break; }
            add_flow = std::min(add_flow, graph[u][previous_edge[v]].residual_capacity);
        }
        if (add_flow <= 0 || add_flow == INF) break;

        for (int v = sink; v != source; v = previous_vertex[v]) {
            int u = previous_vertex[v];
            Edge& edge = graph[u][previous_edge[v]];
            total_cost += add_flow * edge.cost;
            edge.residual_capacity -= add_flow;
            graph[v][edge.reverse_index].residual_capacity += add_flow;
        }
        total_flow += add_flow;
    }
    return {total_flow, total_cost};
}
} // namespace water
