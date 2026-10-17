#pragma once
#include "flow_network.hpp"
#include <utility>

namespace water {
// Returns {flow sent, total cost}.
std::pair<long long, long long> min_cost_max_flow(
    FlowNetwork& network, int source, int sink);
}
