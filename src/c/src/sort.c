#include "debug.h"
#include "heap.h"
#include "partition.h"
#include "search.h"
#include "sort.h"
#include "utils.h"

// These sorting algorithms have signatures `(a, first, last) -> status`, where
// `a[first,last)` is the array to be sorted, and `status` is 1 if the sort is
// successful and 0 otherwise (e.g., if a Python object comparison failed). The
// array `a[first, last)` is sorted in-place.

// Insertion sorts
// Time complexity: O(n^2) worst case
// Space complexity: O(1)

int
InsertionSort(PyObject **a, const Py_ssize_t first, const Py_ssize_t last)
{
    int compare;
    for (Py_ssize_t i = first + 1; i < last; ++i) {
        // Loop invariant: the sub-array `a[first, i)` is sorted. We move `a[i]`
        // into its sorted position within this sub-array by successively
        // swapping adjacent elements.
        for (Py_ssize_t j = i; j > first; --j) {
            if ((compare = LT(a[j], a[j - 1]))) {  // a[j] < a[j - 1]
                if (compare < 0) return 0;  // Comparison failed
                SWAP(a[j], a[j - 1]);
            } else {
                break;
            }
        }
    }
    return 1;
}

int
BinaryInsertionSort(PyObject **a, const Py_ssize_t first,
                      const Py_ssize_t last)
{
    for (Py_ssize_t i = first + 1; i < last; ++i) {
        // Loop invariant: the sub-array `a[first, i)` is sorted, as above
        PyObject *value = a[i];

        // Find where to insert `a[i]` into the sorted list `a[first,i)`
        Py_ssize_t k = BinarySearch(a, value, first, i);
        if (PyErr_Occurred()) {
            // Binary search can fail if the elements aren't comparable
            return 0;
        }
        if (k < i) {
            Py_ssize_t j = i - 1;
            a[i] = a[j];
            for (; k < j; --j)
                a[j] = a[j - 1];
            a[k] = value;
        }
    }
    return 1;
}

// Heap sort
// Time complexity: O(n*log(n)) worst case
// Space complexity: O(1)

int
HeapSort(PyObject **a, const Py_ssize_t first, const Py_ssize_t last)
{
    if (first < last) {
        size_t size = last - first;
        PyObject **heap = a + first;
        if (!BuildBinaryMaxHeap(heap, size)) {
            return 0;
        }
        for (Py_ssize_t heap_end = size - 1; heap_end; --heap_end) {
            SWAP(heap[0], heap[heap_end]);
            if (!BinaryMaxHeapify(heap, 0, heap_end)) {
                return 0;
            }
        }
    }
    return 1;
}

// Merge sort
// Time complexity: O(n*log(n)) worst case
// Space complexity: O(n)
// Paradigm: divide-and-conquer

static int MSort(PyObject **, PyObject **, Py_ssize_t, Py_ssize_t);
static int Merge(PyObject **, PyObject **, Py_ssize_t, Py_ssize_t, Py_ssize_t);

int
MergeSort(PyObject **a, const Py_ssize_t first, const Py_ssize_t last)
{
    int status;
    if (first < last) {
        size_t n = (size_t) (last - first);
        // We're not creating any new Python objects, so using the C allocator
        // should be safe
        PyObject **buf = malloc(n * sizeof(PyObject *));
        if (buf) {
            status = MSort(a, buf, first, last);
            free(buf);
            return status;
        } else {
            PyErr_NoMemory();
            return 0;
        }
    }
    return 1;
}

// Merge sort the array `a[first,last)`, using pre-allocated buffer `buf` to
// store temporary values
static int
MSort(PyObject **a, PyObject **buf, Py_ssize_t first, Py_ssize_t last)
{
    if (first < last - 1) {
        Py_ssize_t mid = first + (last - first) / 2;
        if (!MSort(a, buf, first, mid)) return 0;
        if (!MSort(a, buf, mid, last)) return 0;
        if (!Merge(a, buf, first, mid, last)) return 0;
    }
    return 1;
}

// Combine the sorted arrays `a[first,mid)` and `a[mid,last)` into a sorted
// array `a[first,last)`, using `b` to store a copy of these values
static int
Merge(PyObject **a, PyObject **b, Py_ssize_t first, Py_ssize_t mid,
       Py_ssize_t last)
{
    Py_ssize_t i, j, k;
    Py_ssize_t m = mid - first;
    Py_ssize_t n = last - mid;
    int compare;

    // `b` will store `a[first,mid)`, and `c` will store `a[mid,last)`
    PyObject **c = b + m;
    memcpy(b, a + first, (last - first) * sizeof(PyObject *));

    for (i = 0, j = 0, k = first; (i < m) && (j < n); ++k) {
        if ((compare = LE(b[i], c[j]))) {
            if (compare < 0) return 0;
            a[k] = b[i++];
        } else {
            a[k] = c[j++];
        }
    }

    while (i < m) a[k++] = b[i++];
    return 1;
}

// Quick sorts

static inline int
QSort(PyObject **a, Py_ssize_t first, Py_ssize_t last,
       Py_ssize_t (*partition)(PyObject **, Py_ssize_t, Py_ssize_t))
{
    if (first < last) {
        Py_ssize_t pivot_idx = (*partition)(a, first, last);
        // Partitioning can fail if the elements aren't comparable
        if (PyErr_Occurred()) return 0;
        if (!QSort(a, first, pivot_idx, partition)) return 0;
        if (!QSort(a, pivot_idx + 1, last, partition)) return 0;
    }
    return 1;
}

int QuickSort(PyObject **a, Py_ssize_t first, Py_ssize_t last) {
    return QSort(a, first, last, &Partition);
}

int QuickSortRandom(PyObject **a, Py_ssize_t first, Py_ssize_t last) {
    return QSort(a, first, last, &PartitionRandom);
}

