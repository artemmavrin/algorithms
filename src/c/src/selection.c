#include "debug.h"
#include "selection.h"
#include "utils.h"

// Simultaneously find the minimum and maximum of an array `a` of length `n`
// using `(3/2)n + O(1)` comparisons by iterative over the sequence in pairs:
// first the items in the pair are compared against each other, then the larger
// is compared against the running maximum, and the smaller is compared against
// the running minimum
PyObject *
MinMax(PyObject **a, Py_ssize_t n) {
    PyObject *x, *y, *min, *max;
    Py_ssize_t i;
    int status;

    if (n == 0) {
        PyErr_SetString(PyExc_ValueError, "min_max() arg is empty sequence");
        return NULL;
    }
    if (n % 2) {
        // Initialization when `n` is odd
        min = max = a[0];
        i = 1;
    } else {
        // Initialization when `n` is even
        if ((status = LT(a[0], a[1]))) {
            if (status < 0) return NULL;
            min = a[0];
            max = a[1];
        } else {
            min = a[1];
            max = a[0];
        }
        i = 2;
    }
    for (; i < n; i += 2) {
        x = a[i];
        y = a[i + 1];
        if ((status = LT(x, y))) {
            if (status < 0) return NULL;
            if ((status = LT(x, min))) {
                if (status < 0) return NULL;
                min = x;
            }
            if ((status = GT(y, max))) {
                if (status < 0) return NULL;
                max = y;
            }
        } else {
            if ((status = LT(y, min))) {
                if (status < 0) return NULL;
                min = y;
            }
            if ((status = GT(x, max))) {
                if (status < 0) return NULL;
                max = x;
            }
        }
    }

    return PyTuple_Pack(2, min, max);
}
