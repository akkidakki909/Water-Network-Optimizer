"""Dinic's maximum-flow algorithm."""

from collections import deque
from math import inf
from .graph import FlowNetwork


def dinic_max_flow(network: FlowNetwork, source: int, sink: int) -> int:
    """Compute maximum flow using BFS level graphs and DFS blocking flows.

    The supplied network is modified in place. Use a fresh network for a new run.
    """
    if source == sink:
        return 0
    network._check_node(source)
    network._check_node(sink)
    total_flow = 0
    n = network.n

    while True:
        level = [-1] * n
        level[source] = 0
        queue = deque([source])

        while queue:
            u = queue.popleft()
            for edge in network.adj[u]:
                if edge.residual_capacity > 0 and level[edge.to] == -1:
                    level[edge.to] = level[u] + 1
                    queue.append(edge.to)

        if level[sink] == -1:
            break

        current = [0] * n

        def dfs(u: int, pushed: float) -> int:
            if u == sink:
                return int(pushed)
            while current[u] < len(network.adj[u]):
                edge = network.adj[u][current[u]]
                if edge.residual_capacity > 0 and level[edge.to] == level[u] + 1:
                    amount = dfs(edge.to, min(pushed, edge.residual_capacity))
                    if amount > 0:
                        edge.flow += amount
                        reverse = network.adj[edge.to][edge.rev]
                        reverse.flow -= amount
                        return amount
                current[u] += 1
            return 0

        while True:
            pushed = dfs(source, inf)
            if pushed == 0:
                break
            total_flow += pushed

    return total_flow


def edge_flows(network: FlowNetwork):
    """Return original forward edges as (u, v, flow, capacity, cost)."""
    result = []
    for u, edges in enumerate(network.adj):
        for edge in edges:
            reverse = network.adj[edge.to][edge.rev]
            # A paired edge is forward when its reverse edge has zero capacity
            # and points back to this node; original zero-cost/zero-capacity
            # edges can also appear, but are valid to report.
            if reverse.to == u and reverse.rev < len(network.adj[u]):
                if edge.cost >= 0 and reverse.cost == -edge.cost:
                    if edge.capacity > 0 or (edge.capacity == 0 and edge.cost >= 0):
                        result.append((u, edge.to, edge.flow, edge.capacity, edge.cost))
    return result
