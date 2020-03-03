"""Unit tests for selection algorithms."""

import itertools
import random
import unittest

from algorithms.selection import min_max


class MinMaxTestCase(unittest.TestCase):
    def test_small_permutations(self):
        for n in range(1, 8):
            for a in map(list, itertools.permutations(range(n))):
                a_min, a_max = min_max(a)
                self.assertEqual(a_min, min(a))
                self.assertEqual(a_max, max(a))

    def test_random_large_arrays(self):
        rng = random.Random(0)
        for _ in range(20):
            size = rng.randint(20, 100)
            a = rng.choices(range(50), k=size)
            a_min, a_max = min_max(a)
            self.assertEqual(a_min, min(a))
            self.assertEqual(a_max, max(a))
