import unittest
from water_optimizer.graph import FlowNetwork
from water_optimizer.max_flow import dinic_max_flow


class TestDinic(unittest.TestCase):
    def test_clrs_style_network(self):
        # Classic six-node max-flow example; maximum flow is 23.
        g = FlowNetwork(6)
        edges = [
            (0, 1, 16), (0, 2, 13), (1, 2, 10), (2, 1, 4),
            (1, 3, 12), (3, 2, 9), (2, 4, 14), (4, 3, 7),
            (3, 5, 20), (4, 5, 4),
        ]
        for u, v, c in edges:
            g.add_edge(u, v, c)
        self.assertEqual(dinic_max_flow(g, 0, 5), 23)

    def test_disconnected_sink(self):
        g = FlowNetwork(3)
        g.add_edge(0, 1, 5)
        self.assertEqual(dinic_max_flow(g, 0, 2), 0)

    def test_zero_capacity(self):
        g = FlowNetwork(2)
        g.add_edge(0, 1, 0)
        self.assertEqual(dinic_max_flow(g, 0, 1), 0)

    def test_source_equals_sink(self):
        g = FlowNetwork(1)
        self.assertEqual(dinic_max_flow(g, 0, 0), 0)


if __name__ == "__main__":
    unittest.main()
