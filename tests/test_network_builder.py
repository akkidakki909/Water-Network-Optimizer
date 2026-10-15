import unittest
from water_optimizer.network_builder import build_network, get_pipe_results
from water_optimizer.max_flow import dinic_max_flow
from water_optimizer.min_cost_flow import min_cost_max_flow


NODES = [
    {"id": "A", "type": "source", "amount": 5},
    {"id": "J", "type": "junction"},
    {"id": "Z", "type": "sink", "amount": 4},
]
PIPES = [
    {"from": "A", "to": "J", "capacity": 4, "cost": 2},
    {"from": "J", "to": "Z", "capacity": 3, "cost": 1},
]


class TestBuilder(unittest.TestCase):
    def test_super_source_sink_and_max_flow(self):
        built = build_network(NODES, PIPES)
        self.assertEqual(dinic_max_flow(built.graph, built.source, built.sink), 3)
        results = get_pipe_results(built)
        self.assertEqual(results[1]["flow"], 3)
        self.assertTrue(results[1]["saturated"])

    def test_min_cost_flow(self):
        built = build_network(NODES, PIPES)
        self.assertEqual(min_cost_max_flow(built.graph, built.source, built.sink), (3, 9))


if __name__ == "__main__":
    unittest.main()
