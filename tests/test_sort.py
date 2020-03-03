"""Unit tests for the sorting algorithms."""

import itertools
import random
import unittest
from typing import Callable

from algorithms.sort import binary_insertion_sort
from algorithms.sort import heap_sort
from algorithms.sort import insertion_sort
from algorithms.sort import merge_sort
from algorithms.sort import quick_sort
from algorithms.sort import quick_sort_random

_RNG_SEED = 0
_MAX_PERMUTATION_SIZE = 8
_LARGE_VALUE_POPULATION = range(100, 1000)
_LARGE_SIZE_POPULATION = range(100, 1000)
_N_REPETITIONS = 400


class SortTestCaseMixin:
    sort_fn: Callable
    assertEqual: Callable
    assertIs: Callable
    assertRaisesRegex: Callable
    rng = random.Random(0)

    def _test_sorted(self, _original, _sorted):
        self.assertEqual(_sorted, sorted(_original))

        if isinstance(_original, list):
            # All currently implemented sorting algorithms are inplace on
            # lists; if this changes, there will need to be a switch to turn
            # the next check on or off
            self.assertIs(_sorted, _original)

    def _test_sort_small_seq_no_repetitions(self, sequence_type):
        for n in range(_MAX_PERMUTATION_SIZE):
            for p in map(sequence_type, itertools.permutations(range(n))):
                self._test_sorted(p, self.sort_fn(p))

    def test_sort_small_list_no_repetitions(self):
        self._test_sort_small_seq_no_repetitions(list)

    def test_sort_small_tuple_no_repetitions(self):
        self._test_sort_small_seq_no_repetitions(tuple)

    def test_sort_small_str_no_repetitions(self):
        self._test_sort_small_seq_no_repetitions(str)

    def _test_sort_large_random_seq(self, sequence_type):
        for _ in range(_N_REPETITIONS):
            size = self.rng.choice(_LARGE_SIZE_POPULATION)
            a = self.rng.choices(_LARGE_VALUE_POPULATION, k=size)
            a = sequence_type(a)
            a_sorted = self.sort_fn(a)
            self._test_sorted(a, a_sorted)

    def test_sort_large_random_list(self):
        return self._test_sort_large_random_seq(list)

    def test_sort_large_random_tuple(self):
        return self._test_sort_large_random_seq(tuple)

    def _test_sort_large_random_seq_many_repetitions(self, sequence_type):
        for _ in range(_N_REPETITIONS):
            a = sequence_type(self.rng.choices(range(10), k=1000))
            a_sorted = self.sort_fn(a)
            self._test_sorted(a, a_sorted)

    def test_sort_large_random_list_many_repetitions(self):
        return self._test_sort_large_random_seq_many_repetitions(list)

    def test_sort_large_random_tuple_many_repetitions(self):
        return self._test_sort_large_random_seq_many_repetitions(tuple)

    def test_raises(self):
        # Bad argument types
        with self.assertRaisesRegex(TypeError, 'positional'):
            self.sort_fn(a=[1, 2, 3])
        with self.assertRaisesRegex(TypeError, 'invalid keyword argument'):
            self.sort_fn([0, 1, 2], a=0)
        with self.assertRaisesRegex(TypeError, 'integer is required'):
            self.sort_fn([0, 1, 2], first='abc')
        with self.assertRaisesRegex(TypeError, 'integer is required'):
            self.sort_fn([0, 1, 2], last='abc')

        # Out of range errors
        with self.assertRaisesRegex(IndexError, 'out of range'):
            self.sort_fn([0, 1, 2], first=-1)
        with self.assertRaisesRegex(IndexError, 'out of range'):
            self.sort_fn([0, 1, 2], first=3)
        with self.assertRaisesRegex(IndexError, 'out of range'):
            self.sort_fn([0, 1, 2], last=-1)
        with self.assertRaisesRegex(IndexError, 'out of range'):
            self.sort_fn([0, 1, 2], last=4)

        # Sorting un-sortable things
        with self.assertRaisesRegex(TypeError, 'not supported'):
            self.sort_fn([max, str, int])  # Can't compare functions
        with self.assertRaisesRegex(TypeError, 'not supported'):
            self.sort_fn([0, 'abc'])  # Can't compare int and str
        with self.assertRaisesRegex(TypeError, 'not iterable'):
            self.sort_fn(0)


class BinaryInsertionSortTestCase(SortTestCaseMixin, unittest.TestCase):
    sort_fn = binary_insertion_sort


class InsertionSortTestCase(SortTestCaseMixin, unittest.TestCase):
    sort_fn = insertion_sort


class HeapSortTestCase(SortTestCaseMixin, unittest.TestCase):
    sort_fn = heap_sort


class MergeSortTestCase(SortTestCaseMixin, unittest.TestCase):
    sort_fn = merge_sort


class QuickSortTestCase(SortTestCaseMixin, unittest.TestCase):
    sort_fn = quick_sort


class QuickSortRandomTestCase(SortTestCaseMixin, unittest.TestCase):
    sort_fn = quick_sort_random
