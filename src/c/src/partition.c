#include "debug.h"
#include "partition.h"
#include "utils.h"

#include <time.h>

static int _seeded = 0;

// Partitioning an array `a[first,last)` with respect to `x` means to permute
// the elements of `a[first,last)` such that that every element of
// `a[first,last)` which is less than or equal to `x` occurs before every
// element of `a[first,last)` which is greater than `x`. The functions below
// have signatures (a, first, last) -> i, where, after partitioning, the array
// `a[first, last)` is partitioned with respect to `a[i]` (or `i=first` if
// `first==last`).

// Partition the array `a[first,last)` with respect to `a[first]`
Py_ssize_t
Partition(PyObject **a, const Py_ssize_t first, const Py_ssize_t last)
{
    PyObject *pivot = a[first];
    Py_ssize_t i = last;
    int compare;
    if (first < last) {
        // Start at the right end and grow two disjoint sub-arrays toward the
        // left: the values in `a[i, j)` are less than or equal to `pivot`, and
        // the values in `a[j, last)` are greater than `pivot`. This is the
        // Lomuto partition scheme adapted from Cormen et al.
        // TODO: implement the Hoare partition scheme
        for (Py_ssize_t j = last - 1; j > first; --j) {
            if ((compare = LT(pivot, a[j]))) {  // pivot < a[j]
                if (compare < 0) return 0;  // Comparison failed
                SWAP(a[--i], a[j]);
            }
        }
        SWAP(a[--i], a[first]);
    }
    return i;
}

// Partition the array `a[first,last)` with respect to a random element
Py_ssize_t
PartitionRandom(PyObject **a, const Py_ssize_t first, const Py_ssize_t last) {
    if (!_seeded) {
        srand(time(NULL));
        _seeded = 1;
    }
    if (first < last) {
        Py_ssize_t r = first + (rand() % (last - first));
        DPRINTF("pivot index=%ld\n", r);
        SWAP(a[first], a[r]);
    }
    return Partition(a, first, last);
}
