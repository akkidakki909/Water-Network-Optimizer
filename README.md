# Water & Resource Distribution Network Optimizer — Week 1

This package implements the Python DSA core before adding ML or a web API.

## Structure

```text
water_network_week1/
├── water_optimizer/
│   ├── __init__.py
│   ├── graph.py
│   ├── max_flow.py
│   ├── min_cost_flow.py
│   └── network_builder.py
├── tests/
│   ├── test_max_flow.py
│   ├── test_min_cost_flow.py
│   └── test_network_builder.py
├── requirements.txt
└── README.md
```

## Requirements

Python 3.10+ recommended. Week 1 uses only the standard library.

## Run tests

Open a terminal in the `water_network_week1` folder:

```bash
python -m unittest discover -s tests -v
```

Expected result: all tests pass.

## Quick example

```python
from water_optimizer.network_builder import build_network, get_pipe_results
from water_optimizer.max_flow import dinic_max_flow
from water_optimizer.min_cost_flow import min_cost_max_flow

nodes = [
    {"id": "ReservoirA", "type": "source", "amount": 10},
    {"id": "J1", "type": "junction"},
    {"id": "Zone1", "type": "sink", "amount": 8},
]
pipes = [
    {"from": "ReservoirA", "to": "J1", "capacity": 7, "cost": 2},
    {"from": "J1", "to": "Zone1", "capacity": 6, "cost": 3},
]

built = build_network(nodes, pipes)
print("Max flow:", dinic_max_flow(built.graph, built.source, built.sink))
print("Pipe results:", get_pipe_results(built))

# Build a fresh network for a separate algorithm run because algorithms
# modify the residual graph in place.
built = build_network(nodes, pipes)
print("Min-cost max-flow:",
      min_cost_max_flow(built.graph, built.source, built.sink))
```

## DSA design

- `graph.py`: adjacency-list representation with paired forward/reverse residual edges.
- `max_flow.py`: Dinic — BFS level graph, DFS blocking flow, current-arc array.
- `min_cost_flow.py`: Bellman-Ford initial potentials followed by Dijkstra with `heapq`.
- `network_builder.py`: converts named water sources, junctions, destinations and pipes into a graph with a super-source and super-sink.

## Important behavior

- Both algorithms modify the graph's flow values. Build a fresh network for every independent run.
- Max-flow maximizes delivery, but does not minimize cost.
- Min-cost max-flow maximizes flow first, then minimizes cost among maximum-flow solutions.
- Costs are integer per-unit costs. The Week 1 version assumes no reachable negative-cost cycle in the initial input network.
- `total_demand` is the sum of sink demand limits; `flow / total_demand` can be used to calculate demand met percentage (guard against total demand being zero).
- This is an algorithm prototype, not a hydraulic simulator. Pipe capacity and cost are simplified graph parameters.
