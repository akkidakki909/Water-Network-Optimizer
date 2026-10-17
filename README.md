# Water & Resource Distribution Network Optimizer — Week 1 (C++)

This is the **C++17** version of the Week 1 DSA engine. The focus stays on graph algorithms; AI/ML can be added later as a separate demand-forecasting feature.

## Structure

```text
Water_Network_Optimizer_CPP_Week1/
├── include/
│   ├── flow_network.hpp
│   ├── dinic.hpp
│   ├── min_cost_flow.hpp
│   └── network_builder.hpp
├── src/
│   ├── flow_network.cpp
│   ├── dinic.cpp
│   ├── min_cost_flow.cpp
│   ├── network_builder.cpp
│   └── main.cpp
├── tests/
│   └── test_main.cpp
├── CMakeLists.txt
└── README.md
```

## Requirements

- A C++17 compiler: GCC/g++, Clang, or MSVC.
- No external libraries are required.

## Build and run (g++)

Open a terminal in this folder:

```bash
g++ -std=c++17 -Wall -Wextra -pedantic -Iinclude src/*.cpp -o water_optimizer
./water_optimizer
```

On Windows Command Prompt, run the executable with:

```bat
water_optimizer.exe
```

## Run tests

Compile the test file separately from `src/main.cpp`:

```bash
g++ -std=c++17 -Wall -Wextra -pedantic -Iinclude src/flow_network.cpp src/dinic.cpp src/min_cost_flow.cpp src/network_builder.cpp tests/test_main.cpp -o water_optimizer_tests
```

Then run `water_optimizer_tests` (or `water_optimizer_tests.exe` on Windows). The tests use simple assertions and need no testing framework.

## Algorithms included

- **Adjacency list + residual edges:** every forward edge has a paired reverse edge.
- **Dinic max-flow:** BFS level graph, DFS blocking flow, current-arc optimization.
- **Min-cost max-flow:** Bellman-Ford initial potentials, then Dijkstra using `std::priority_queue` and reduced costs.
- **Multiple sources and destinations:** super-source and super-sink reduction.
- **Pipe usage reporting:** original pipe edges are tagged and their final flow can be inspected.

## Important notes

- Build a fresh network for each independent algorithm run because flow algorithms mutate residual capacities.
- Max-flow maximizes delivered quantity; it does not minimize cost.
- Min-cost max-flow first sends as much flow as possible, then minimizes cost for that amount.
- Costs/capacities are integers. This is a graph model for the course project, not a hydraulic simulation.
- The initial input network should not contain a reachable negative-cost cycle.
