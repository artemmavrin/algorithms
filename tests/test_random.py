"""Unit tests for the miscellaneous algorithms."""

import itertools
import math
import random
import unittest

from algorithms.random import sample


class SampleTestCase(unittest.TestCase):
    def test_bad_input(self):
        # Not a nonnegative integer size
        with self.assertRaises(TypeError):
            sample(range(10), size=1.0)
        with self.assertRaises(ValueError):
            sample(range(10), size=-1)

        # Population not iterable
        with self.assertRaises(TypeError):
            sample(0)
        with self.assertRaises(TypeError):
            sample(0.5)

        # Population must be larger than the sample size
        with self.assertRaises(ValueError):
            sample(range(10), size=11)

    def test_range(self):
        for n, k in itertools.product((100, 1000, 10000), (1, 10, 100)):
            for population in (range(n), (i for i in range(n)), list(range(n))):
                data = sample(population, size=k, seed=0)
                self.assertIsInstance(data, list)
                for item in data:
                    self.assertTrue(0 <= item < n)

    def test_distribution(self):
        # Test parameters
        n_repeats = 20000
        delta = 2e-2

        for length in (2, 3, 4):
            # The population to sample from
            population = range(length)

            for size in range(1, length + 1):
                # Get all subsets of the population of the given size
                subsets = list(itertools.combinations(population, size))

                # Create a dictionary to keep track of each subset's frequency
                subsets = {subset: 0 for subset in subsets}

                # Sample many random subsets
                for _ in range(n_repeats):
                    subset = sample(population, size=size)
                    subset = tuple(sorted(subset))
                    subsets[subset] += 1

                # Compute estimates for the probability of each subset being
                # drawn. Note: this relies on dicts being insertion ordered,
                # which is guaranteed from Python 3.7 onward (and was a CPython
                # implementation detail in Python 3.6)
                probs = [count / n_repeats for count in subsets.values()]

                # The probabilities should all be approximately uniform
                n_subsets = len(subsets)
                for prob in probs:
                    self.assertAlmostEqual(prob, 1 / n_subsets, delta=delta)
