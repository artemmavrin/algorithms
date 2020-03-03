"""Graph algorithm unit tests."""

import unittest
from collections import deque
from itertools import combinations, product
from typing import Any, Iterable, Iterator, Mapping

from algorithms.graph import bfs

SIMPLE_GRAPHS = [
    {0: [1, 2], 1: [3, 4], 2: [5, 6], 4: [4, 5, 6, 0]},
    # The empty graph
    {},
    # Self-loops should be okay
    {0: [0]},
    # A linked list
    {0: [1], 1: [2], 3: [4], 4: [5]},
    # Petersen graph
    {0: [4, 1, 5], 1: [0, 2, 6], 2: [1, 3, 7], 3: [2, 4, 8], 4: [3, 0, 9],
     5: [0, 7, 8], 6: [1, 8, 9], 7: [2, 9, 5], 8: [3, 5, 6], 9: [4, 7, 8]},
    # Guido's graph from https://www.python.org/doc/essays/graphs/
    {'A': ['B', 'C'], 'B': ['C', 'D'], 'C': ['D'], 'D': ['C'], 'E': ['F'],
     'F': ['C']},
    # A small complete bipartite graph
    {0: [1, 3, 5], 1: [0, 2], 2: [1, 3, 5], 3: [0, 2], 4: [1, 3, 5], 5: [0, 2]},
]


def bfs_py(graph: Mapping[Any, Iterable[Any]], root: Any) -> Iterator[Any]:
    """Pure Python reference breadth-first search implementation."""
    queue = deque()
    seen = set()
    queue.append(root)
    while queue:
        node = queue.popleft()
        if node not in seen:
            seen.add(node)
            neighbors = graph.get(node, None)
            if neighbors is not None:
                queue.extend(neighbors)
            yield node


def all_sequences(n):
    for i in range(n + 1):
        yield from combinations(range(n), i)


def all_graphs(n):
    for tuples in product(all_sequences(n), repeat=n):
        yield dict(enumerate(tuples))


def vertices(graph: dict):
    return set(graph).union(*graph.values())


class BFSTestCase(unittest.TestCase):
    def test_small_graphs(self):
        for n in range(1, 5):
            for graph, root in product(all_graphs(n), range(n)):
                b1 = list(bfs(graph, root))
                b2 = list(bfs_py(graph, root))
                self.assertEqual(b1, b2)
                for v1, v2 in zip(b1, b2):
                    self.assertIs(v1, v2)

    def test_simple_graphs(self):
        for graph in SIMPLE_GRAPHS:
            for root in {None}.union(vertices(graph)):
                b1 = list(bfs(graph, root))
                b2 = list(bfs_py(graph, root))
                self.assertEqual(b1, b2)
                for v1, v2 in zip(b1, b2):
                    self.assertIs(v1, v2)
