#include "debug.h"
#include "search.h"
#include "utils.h"

// Binary search: returns the index `idx` in the range `[first,last]` such that
// every element in `a[first,idx)` is less than or equal to `value` and every
// element in `a[idx,last)` is greater than `value`
//
// Precondition: `a[first,last)` is partitioned with respect to `value`, meaning
// that every element of `a[first,last)` which is less than or equal to `value`
// occurs before every element of `a[first,last)` which is greater than `value`.
// In particular, this is true if `a[first,last)` is sorted.
Py_ssize_t
BinarySearch(PyObject **a, PyObject *value, Py_ssize_t first, Py_ssize_t last)
{
    Py_ssize_t midpoint;
    int status;

    // Iterative implementation of the textbook tail recursive algorithm
    while (1) {
        if (last <= first) {
            status = LE(a[first], value);
            if (status < 0) return 0;  // Comparison failed
            return first + status;
        }

        // Compute the midpoint between first and last, avoiding overflow
        midpoint = first + (last - first) / 2;

        if ((status = EQ(a[midpoint], value))) {
            // Advance the midpoint to the first position where the item is
            // greater than `value`
            do {
                if (status < 0) return 0;  // Comparison failed
                ++midpoint;
            } while (midpoint < last && (status = EQ(a[midpoint], value)));
            return midpoint;
        } else if ((status = LT(a[midpoint], value))) {
            if (status < 0) return 0;  // Comparison failed
            first = midpoint + 1;
        } else {
            last = midpoint;
        }
    }
}
