"""Minimum-cost maximum-flow using potentials + heap-based Dijkstra."""

from heapq import heappush, heappop
from math import inf
from .graph import FlowNetwork


def _initial_potentials(network: FlowNetwork, source: int):
    """Bellman-Ford initialization for graphs with negative residual costs."""
    n = network.n
    dist = [inf] * n
    dist[source] = 0

    for _ in range(n - 1):
        changed = False
        for u in range(n):
            if dist[u] == inf:
                continue
            for edge in network.adj[u]:
                if edge.residual_capacity > 0 and dist[edge.to] > dist[u] + edge.cost:
                    dist[edge.to] = dist[u] + edge.cost
                    changed = True
        if not changed:
            break

    return [0 if value == inf else value for value in dist]


def min_cost_max_flow(network: FlowNetwork, source: int, sink: int):
    """Return (maximum_flow, minimum_cost_for_that_flow).

    Uses Bellman-Ford once to initialize potentials, then Dijkstra on reduced
    costs with a binary heap. The network is modified in place.
    """
    network._check_node(source)
    network._check_node(sink)
    if source == sink:
        return 0, 0

    n = network.n
    potential = _initial_potentials(network, source)
    total_flow = 0
    total_cost = 0

    while True:
        dist = [inf] * n
        parent_node = [-1] * n
        parent_edge = [-1] * n
        dist[source] = 0
        heap = [(0, source)]

        while heap:
            current_dist, u = heappop(heap)
            if current_dist != dist[u]:
                continue
            for edge_index, edge in enumerate(network.adj[u]):
                if edge.residual_capacity <= 0:
                    continue
                reduced_cost = edge.cost + potential[u] - potential[edge.to]
                candidate = current_dist + reduced_cost
                if candidate < dist[edge.to]:
                    dist[edge.to] = candidate
                    parent_node[edge.to] = u
                    parent_edge[edge.to] = edge_index
                    heappush(heap, (candidate, edge.to))

        if dist[sink] == inf:
            break

        for node in range(n):
            if dist[node] < inf:
                potential[node] += dist[node]

        amount = float("inf")
        node = sink
        while node != source:
            u = parent_node[node]
            if u == -1:
                raise RuntimeError("Broken predecessor chain while augmenting")
            edge = network.adj[u][parent_edge[node]]
            amount = min(amount, edge.residual_capacity)
            node = u
        amount = int(amount)

        node = sink
        while node != source:
            u = parent_node[node]
            edge = network.adj[u][parent_edge[node]]
            edge.flow += amount
            network.adj[edge.to][edge.rev].flow -= amount
            total_cost += amount * edge.cost
            node = u

        total_flow += amount

    return total_flow, total_cost
