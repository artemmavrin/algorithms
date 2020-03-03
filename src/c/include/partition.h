#ifndef __ALGORITHMS_PARTITION_H
#define __ALGORITHMS_PARTITION_H

#define PY_SSIZE_T_CLEAN
#include <Python.h>

Py_ssize_t Partition(PyObject **, const Py_ssize_t, const Py_ssize_t);
Py_ssize_t PartitionRandom(PyObject **, const Py_ssize_t, const Py_ssize_t);

#endif
