"""Directed flow network with paired forward/reverse residual edges."""

from dataclasses import dataclass
from typing import List


@dataclass
class Edge:
    to: int
    rev: int
    capacity: int
    cost: int = 0
    flow: int = 0

    @property
    def residual_capacity(self) -> int:
        return self.capacity - self.flow


class FlowNetwork:
    """Adjacency-list graph. Each added edge has a paired reverse edge."""

    def __init__(self, node_count: int):
        if node_count < 0:
            raise ValueError("node_count must be non-negative")
        self.n = node_count
        self.adj: List[List[Edge]] = [[] for _ in range(node_count)]

    def add_edge(self, u: int, v: int, capacity: int, cost: int = 0) -> None:
        """Add a directed edge and its zero-capacity residual reverse edge."""
        self._check_node(u)
        self._check_node(v)
        if capacity < 0:
            raise ValueError("capacity must be non-negative")
        forward = Edge(to=v, rev=len(self.adj[v]), capacity=capacity, cost=cost)
        reverse = Edge(to=u, rev=len(self.adj[u]), capacity=0, cost=-cost)
        self.adj[u].append(forward)
        self.adj[v].append(reverse)

    def _check_node(self, node: int) -> None:
        if not isinstance(node, int) or not 0 <= node < self.n:
            raise IndexError(f"node index {node!r} is outside 0..{self.n - 1}")

    def clone(self) -> "FlowNetwork":
        """Return a fresh network with the same original edges and no flow."""
        copied = FlowNetwork(self.n)
        for u in range(self.n):
            for edge in self.adj[u]:
                # Only copy forward/original edges. Reverse edges have cap=0,
                # but original zero-capacity edges are harmless if copied twice;
                # use reverse-index ordering to copy each pair once.
                if edge.rev >= 0 and edge.to >= 0:
                    reverse = self.adj[edge.to][edge.rev]
                    if reverse.to == u and reverse.rev < len(self.adj[u]):
                        # A forward edge is the member whose paired reverse
                        # points back to u and has the reciprocal index.
                        if edge.cost >= 0 and reverse.cost == -edge.cost:
                            pass
                # Original edges are identified by each pair's insertion order.
                # The implementation below uses the adjacency location test.
                if edge.capacity > 0 or (edge.capacity == 0 and edge.cost >= 0):
                    reverse = self.adj[edge.to][edge.rev]
                    if reverse.to == u and reverse.rev < len(self.adj[u]):
                        if reverse.rev == self.adj[u].index(edge):
                            copied.add_edge(u, edge.to, edge.capacity, edge.cost)
        return copied

    def reset_flows(self) -> None:
        """Reset all flows, including reverse-edge flows, to zero."""
        for edges in self.adj:
            for edge in edges:
                edge.flow = 0
