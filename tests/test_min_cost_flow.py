import unittest
from water_optimizer.graph import FlowNetwork
from water_optimizer.min_cost_flow import min_cost_max_flow


class TestMinCostMaxFlow(unittest.TestCase):
    def test_known_flow_and_cost(self):
        # Two routes, each capacity 2; cheapest route costs 1/unit, other 3/unit.
        g = FlowNetwork(4)
        g.add_edge(0, 1, 2, 1)
        g.add_edge(1, 3, 2, 0)
        g.add_edge(0, 2, 2, 3)
        g.add_edge(2, 3, 2, 0)
        self.assertEqual(min_cost_max_flow(g, 0, 3), (4, 8))

    def test_reverse_edges_allow_rerouting(self):
        # The optimum requires residual reverse edges to undo an earlier choice.
        g = FlowNetwork(4)
        g.add_edge(0, 1, 1, 0)
        g.add_edge(0, 2, 1, 0)
        g.add_edge(1, 2, 1, 0)
        g.add_edge(1, 3, 1, 1)
        g.add_edge(2, 3, 1, 0)
        flow, cost = min_cost_max_flow(g, 0, 3)
        self.assertEqual(flow, 2)
        self.assertEqual(cost, 1)

    def test_no_path(self):
        g = FlowNetwork(3)
        g.add_edge(0, 1, 3, 2)
        self.assertEqual(min_cost_max_flow(g, 0, 2), (0, 0))


if __name__ == "__main__":
    unittest.main()
