#ifndef __ALGORITHMS_SORT_H
#define __ALGORITHMS_SORT_H

#define PY_SSIZE_T_CLEAN
#include <Python.h>

int BinaryInsertionSort(PyObject **, const Py_ssize_t, const Py_ssize_t);
int InsertionSort(PyObject **, const Py_ssize_t, const Py_ssize_t);
int HeapSort(PyObject **, const Py_ssize_t, const Py_ssize_t);
int MergeSort(PyObject **, const Py_ssize_t, const Py_ssize_t);
int QuickSort(PyObject **, const Py_ssize_t, const Py_ssize_t);
int QuickSortRandom(PyObject **, const Py_ssize_t, const Py_ssize_t);

#endif
