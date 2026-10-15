"""Build a single-source/single-sink flow network from a water network.

Input schema:
{
  "nodes": [
    {"id": "ReservoirA", "type": "source", "amount": 20},
    {"id": "Zone1", "type": "sink", "amount": 18},
    {"id": "J1", "type": "junction"}
  ],
  "pipes": [
    {"from": "ReservoirA", "to": "J1", "capacity": 12, "cost": 2}
  ]
}
"""

from dataclasses import dataclass
from .graph import FlowNetwork


@dataclass
class BuiltNetwork:
    graph: FlowNetwork
    source: int
    sink: int
    node_ids: list
    id_to_index: dict
    pipe_edges: list
    super_source: int
    super_sink: int
    total_demand: int


def build_network(nodes, pipes) -> BuiltNetwork:
    ids = [node["id"] for node in nodes]
    if len(set(ids)) != len(ids):
        raise ValueError("Node IDs must be unique")
    if not ids:
        raise ValueError("At least one node is required")

    index = {node_id: i for i, node_id in enumerate(ids)}
    super_source = len(ids)
    super_sink = len(ids) + 1
    graph = FlowNetwork(len(ids) + 2)
    pipe_edges = []

    for pipe in pipes:
        u_id, v_id = pipe["from"], pipe["to"]
        if u_id not in index or v_id not in index:
            raise ValueError(f"Pipe references unknown node: {u_id!r} -> {v_id!r}")
        capacity = int(pipe["capacity"])
        cost = int(pipe.get("cost", 0))
        if capacity < 0:
            raise ValueError("Pipe capacity must be non-negative")
        u, v = index[u_id], index[v_id]
        edge_index = len(graph.adj[u])
        graph.add_edge(u, v, capacity, cost)
        pipe_edges.append((u, edge_index, u_id, v_id))

    total_demand = 0
    for node in nodes:
        node_type = node.get("type", "junction").lower()
        amount = int(node.get("amount", 0))
        if amount < 0:
            raise ValueError(f"Amount for node {node['id']} must be non-negative")
        idx = index[node["id"]]
        if node_type == "source":
            graph.add_edge(super_source, idx, amount, 0)
        elif node_type == "sink":
            graph.add_edge(idx, super_sink, amount, 0)
            total_demand += amount
        elif node_type != "junction":
            raise ValueError(f"Unknown node type {node_type!r} for {node['id']!r}")

    return BuiltNetwork(
        graph=graph,
        source=super_source,
        sink=super_sink,
        node_ids=ids,
        id_to_index=index,
        pipe_edges=pipe_edges,
        super_source=super_source,
        super_sink=super_sink,
        total_demand=total_demand,
    )


def get_pipe_results(built: BuiltNetwork):
    """Return per-pipe flow information in the same order as input pipes."""
    results = []
    for u, edge_index, u_id, v_id in built.pipe_edges:
        edge = built.graph.adj[u][edge_index]
        results.append({
            "from": u_id,
            "to": v_id,
            "flow": edge.flow,
            "capacity": edge.capacity,
            "cost_per_unit": edge.cost,
            "total_cost": edge.flow * edge.cost,
            "saturated": edge.capacity > 0 and edge.flow == edge.capacity,
        })
    return results
