#ifndef __ALGORITHMS_UTILS_H
#define __ALGORITHMS_UTILS_H

#define PY_SSIZE_T_CLEAN
#include <Python.h>

void Swap(PyObject **, PyObject **);

// Swap the values of two lvalue expressions a and b of type PyObject *
#define SWAP(a, b) Swap(&(a), &(b))

// Comparison shortcuts, returning 0 if False, 1 if True, -1 if failure
#define EQ(a, b) PyObject_RichCompareBool((a), (b), Py_EQ)  // a == b
#define GT(a, b) PyObject_RichCompareBool((a), (b), Py_GT)  // a > b
#define GE(a, b) PyObject_RichCompareBool((a), (b), Py_GE)  // a >= b
#define LT(a, b) PyObject_RichCompareBool((a), (b), Py_LT)  // a < b
#define LE(a, b) PyObject_RichCompareBool((a), (b), Py_LE)  // a <= b

#endif
