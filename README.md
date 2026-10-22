# Water & Resource Distribution Network Optimizer — Week 2 (C++17)

Week 2 extends the C++ DSA engine with a demand-forecasting feature. Graph algorithms remain the core; forecasting estimates destination demand and passes the rounded estimate into the network optimizer.

## Project structure

```text
Water_Network_Optimizer_CPP_Week2/
├── include/       # graph, max-flow, min-cost flow, and forecasting headers
├── src/           # C++ implementations and demo main.cpp
├── tests/         # graph algorithm tests and demand forecasting tests
├── CMakeLists.txt
└── README.md
```

## Requirements

- A C++17 compiler such as MinGW-w64 `g++`, `clang++`, or Visual Studio C++ tools.
- No third-party libraries are required.

Check that `g++` is installed:

```bash
g++ --version
```

## How to run on Windows (VS Code)

1. Extract the ZIP.
2. Open the extracted `Water_Network_Optimizer_CPP_Week2` folder in VS Code.
3. Select **Terminal → New Terminal**. Run commands from the project root (the folder containing `README.md`, `include`, `src`, and `tests`).
4. Compile the demo:

```powershell
g++ -std=c++17 -Wall -Wextra -pedantic -Iinclude src/*.cpp -o water_optimizer.exe
```

5. Run it:

```powershell
.\water_optimizer.exe
```

The demo predicts demand for two zones from sample history, then runs Dinic maximum-flow and min-cost max-flow using the forecasts as destination demand limits.

## How to run on Linux/macOS

From the project root:

```bash
g++ -std=c++17 -Wall -Wextra -pedantic -Iinclude src/*.cpp -o water_optimizer
./water_optimizer
```

## Run tests

Compile and run the graph algorithm tests separately (do not include `src/main.cpp`):

**Windows PowerShell**
```powershell
g++ -std=c++17 -Wall -Wextra -pedantic -Iinclude src/flow_network.cpp src/dinic.cpp src/min_cost_flow.cpp src/network_builder.cpp tests/test_main.cpp -o water_tests.exe
.\water_tests.exe
```

Compile and run the Week 2 forecasting tests:

```powershell
g++ -std=c++17 -Wall -Wextra -pedantic -Iinclude src/demand_forecast.cpp tests/test_demand_forecast.cpp -o forecast_tests.exe
.\forecast_tests.exe
```

On Linux/macOS, use the same compile commands but omit `.exe` from output names and run with `./water_tests` and `./forecast_tests`.

Expected messages: `All 4 C++ tests passed.` and `All 4 demand forecasting tests passed.`

## Optional CMake build

```bash
cmake -S . -B build
cmake --build build
ctest --test-dir build --output-on-failure
```

## Week 2 feature: demand forecasting

`demand_forecast.cpp` implements least-squares linear regression over `(time_step, demand)` observations. It returns the predicted demand, fitted slope/intercept, and a rounded non-negative integer demand limit for the graph optimizer. The included observations are synthetic examples.

This is an interpretable forecasting **baseline**, not a Random Forest or production-grade model. In a later iteration, it can be compared with a richer model using realistic historical demand and prediction-error metrics. The core DSA remains adjacency-list graph representation, Dinic max-flow, min-cost max-flow, residual edges, and super-source/super-sink reduction.

## Notes

- Flow algorithms mutate residual capacities, so the demo builds a fresh network for each independent run.
- Maximum flow maximizes quantity; min-cost max-flow minimizes cost for the maximum flow it can send.
- Capacities and costs are integer units. This is a graph model, not a physical hydraulic simulation.
- The min-cost flow implementation assumes no reachable negative-cost cycle in the initial network.
