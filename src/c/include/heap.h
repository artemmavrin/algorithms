#ifndef __ALGORITHMS_HEAP_H
#define __ALGORITHMS_HEAP_H

#define PY_SSIZE_T_CLEAN
#include <Python.h>

#define BINARY_HEAP_PARENT(i) (((i) - 1) / 2)
#define BINARY_HEAP_LCHILD(i) (2 * (i) + 1)
#define BINARY_HEAP_RCHILD(i) (2 * (i) + 2)

int BinaryMinHeapify(PyObject **, Py_ssize_t, Py_ssize_t);
int BinaryMaxHeapify(PyObject **, Py_ssize_t, Py_ssize_t);
int BuildBinaryMinHeap(PyObject **, Py_ssize_t);
int BuildBinaryMaxHeap(PyObject **, Py_ssize_t);

#endif
