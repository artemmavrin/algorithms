#include "debug.h"
#include "heap.h"
#include "utils.h"

// Turn the binary sub-tree of the array `a[0,size)` rooted at index `root` into
// a binary heap, assuming that its left and right sub-trees are already heaps
static inline int
BinaryHeapify(PyObject **a, Py_ssize_t root, Py_ssize_t size, int op) {
    Py_ssize_t l, r, i;
    int status;

    // The heapify procedure described in Cormen et al is naturally tail
    // recursive, so we just unroll it to save function calls
    while (1) {
        l = BINARY_HEAP_LCHILD(root);
        r = BINARY_HEAP_RCHILD(root);

        // Get the index of the largest/smallest element among the root node at
        // index `root` and the children at the left and right indices
        i = root;
        if ((l < size) && (status = PyObject_RichCompareBool(a[l], a[i], op))) {
            if (status < 0) return 0;  // PyObject comparison failed
            i = l;
        }
        if ((r < size) && (status = PyObject_RichCompareBool(a[r], a[i], op))) {
            if (status < 0) return 0;  // PyObject comparison failed
            i = r;
        }
        if (i == root) {
            // The heap property is satisfied at the root node
            return 1;
        } else {
            // We found a child larger/smaller than its parent; swap it with its
            // parent and make the sub-tree rooted at the larger/smaller child
            // node into a heap
            SWAP(a[root], a[i]);
            root = i;
        }
    }
}

// Permute the elements of `a[0,size)` into a binary heap
static inline int
BuildBinaryHeap(PyObject **a, Py_ssize_t size, int op)
{
    // Construct the heap bottom-up by starting heapifying the subtrees at the
    // second-to-last level and working up toward the top (the subtrees at the
    // bottom level are vacuously heaps)
    for (Py_ssize_t i = BINARY_HEAP_PARENT(size - 1); i >= 0; --i)
        if (!BinaryHeapify(a, i, size, op))
            return 0;  // PyObject comparison failed during heapification
    return 1;
}

int
BinaryMinHeapify(PyObject **a, Py_ssize_t root, Py_ssize_t size)
{
    return BinaryHeapify(a, root, size, Py_LT);
}

int
BinaryMaxHeapify(PyObject **a, Py_ssize_t root, Py_ssize_t size)
{
    return BinaryHeapify(a, root, size, Py_GT);
}

int
BuildBinaryMinHeap(PyObject **a, Py_ssize_t size)
{
    return BuildBinaryHeap(a, size, Py_LT);
}

int
BuildBinaryMaxHeap(PyObject **a, Py_ssize_t size)
{
    return BuildBinaryHeap(a, size, Py_GT);
}
