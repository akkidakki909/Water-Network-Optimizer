#pragma once
#include "flow_network.hpp"
#include <vector>

namespace water {
long long dinic_max_flow(FlowNetwork& network, int source, int sink);
}
