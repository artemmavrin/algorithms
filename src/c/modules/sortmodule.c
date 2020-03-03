#define PY_SSIZE_T_CLEAN
#include <Python.h>

#include "debug.h"
#include "sort.h"

// Helper function used by sort_boilerplate to parse `first` and `last`
static inline void
parse_index(PyObject *op, Py_ssize_t *index, Py_ssize_t default_,
            Py_ssize_t max) {
    if (op == NULL || op == Py_None) {
        *index = default_;
    } else {
        *index = PyLong_AsSsize_t(op);
        // PyLong_AsSsize_t returns -1 on failure, so an error check is needed
        if (PyErr_Occurred())
            return;
        if ((size_t) index > (size_t) max) {
            PyErr_SetString(PyExc_IndexError, "Index out of range.");
            return;
        }
    }
}

// Wrapper around sorting implementations that handles things like argument
// parsing and initial error checking
static inline PyObject *
sort_boilerplate(PyObject *self, PyObject *args, PyObject *kwargs,
                 int (*sort)(PyObject **, const Py_ssize_t, const Py_ssize_t))
{
    // PyObjects are guilty until proven innocent
    PyObject *obj = NULL;
    PyObject *first = NULL;
    PyObject *last = NULL;

    Py_ssize_t size;
    Py_ssize_t _first;
    Py_ssize_t _last;

    (void) self;  // Unused parameter

    // Parse positional and keyword arguments
    static const char *format = "O|OO";
    static char *keywords[] = {"", "first", "last", NULL};
    if (!PyArg_ParseTupleAndKeywords(args, kwargs, format, keywords, &obj,
                                     &first, &last))
        return NULL;

    // If `obj` is a list, sort it in-place. In every other case, interpret
    // `obj` as a sequence, and sort the list of its values
    if (PyList_CheckExact(obj)) {
        Py_INCREF(obj);
    } else {
        obj = PySequence_List(obj);
        if (obj == NULL) return NULL;
    }

    // Figure out sorting bounds, defaulting to the entire list
    size = PyList_GET_SIZE(obj);
    parse_index(first, &_first, 0, size - 1);
    parse_index(last, &_last, size, size);
    if (PyErr_Occurred()) {
        Py_DECREF(obj);
        return NULL;
    }

    // Actual sorting is handled by the function implementing a particular
    // algorithm
    if (!sort(((PyListObject *) obj)->ob_item, _first, _last)) {
        // Sorting may fail if, e.g., elements in the list aren't comparable
        Py_DECREF(obj);
        return NULL;
    }
    return obj;
}

#define SORT_SIGNATURE(name) name "(seq) -> list\n\n"
#define COMMON_SORT_DOC \
"If the input is a list, then it is sorted in-place and returned. Otherwise\n" \
"a new list containing the items of the input in sorted order is returned."

static PyObject *
Sort_BinaryInsertionSort(PyObject *self, PyObject *args, PyObject *kwargs)
{
    return sort_boilerplate(self, args, kwargs, &BinaryInsertionSort);
}

static PyObject *
Sort_InsertionSort(PyObject *self, PyObject *args, PyObject *kwargs)
{
    return sort_boilerplate(self, args, kwargs, &InsertionSort);
}

static PyObject *
Sort_HeapSort(PyObject *self, PyObject *args, PyObject *kwargs)
{
    return sort_boilerplate(self, args, kwargs, &HeapSort);
}

static PyObject *
Sort_MergeSort(PyObject *self, PyObject *args, PyObject *kwargs)
{
    return sort_boilerplate(self, args, kwargs, &MergeSort);
}

static PyObject *
Sort_QuickSort(PyObject *self, PyObject *args, PyObject *kwargs)
{
    return sort_boilerplate(self, args, kwargs, &QuickSort);
}

static PyObject *
Sort_QuickSortRandom(PyObject *self, PyObject *args, PyObject *kwargs)
{
    return sort_boilerplate(self, args, kwargs, &QuickSortRandom);
}

// Python function names
#define BINARY_INSERTION_SORT "binary_insertion_sort"
#define INSERTION_SORT "insertion_sort"
#define HEAP_SORT "heap_sort"
#define MERGE_SORT "merge_sort"
#define QUICK_SORT "quick_sort"
#define QUICK_SORT_RANDOM "quick_sort_random"

// Docstrings

PyDoc_STRVAR(Sort_BinaryInsertionSort_doc,
SORT_SIGNATURE(BINARY_INSERTION_SORT)
"Binary insertion sort.\n"
COMMON_SORT_DOC);

PyDoc_STRVAR(Sort_InsertionSort_doc,
SORT_SIGNATURE(INSERTION_SORT)
"Insertion sort.\n"
COMMON_SORT_DOC);

PyDoc_STRVAR(Sort_HeapSort_doc,
SORT_SIGNATURE(HEAP_SORT)
"Heap sort.\n"
COMMON_SORT_DOC);

PyDoc_STRVAR(Sort_MergeSort_doc,
SORT_SIGNATURE(MERGE_SORT)
"Merge sort.\n"
COMMON_SORT_DOC);

PyDoc_STRVAR(Sort_QuickSort_doc,
SORT_SIGNATURE(QUICK_SORT)
"Quick sort.\n"
COMMON_SORT_DOC);

PyDoc_STRVAR(Sort_QuickSortRandom_doc,
SORT_SIGNATURE(QUICK_SORT_RANDOM)
"Randomized quick sort.\n"
COMMON_SORT_DOC);

// List of functions exposed by the module
static PyMethodDef SortMethods[] = {
    {
        .ml_name = BINARY_INSERTION_SORT,
        .ml_meth = (PyCFunction) Sort_BinaryInsertionSort,
        .ml_flags = METH_VARARGS | METH_KEYWORDS,
        .ml_doc = Sort_BinaryInsertionSort_doc,
    },
    {
        .ml_name = INSERTION_SORT,
        .ml_meth = (PyCFunction) Sort_InsertionSort,
        .ml_flags = METH_VARARGS | METH_KEYWORDS,
        .ml_doc = Sort_InsertionSort_doc,
    },
    {
        .ml_name = HEAP_SORT,
        .ml_meth = (PyCFunction) Sort_HeapSort,
        .ml_flags = METH_VARARGS | METH_KEYWORDS,
        .ml_doc = Sort_HeapSort_doc,
    },
    {
        .ml_name = MERGE_SORT,
        .ml_meth = (PyCFunction) Sort_MergeSort,
        .ml_flags = METH_VARARGS | METH_KEYWORDS,
        .ml_doc = Sort_MergeSort_doc,
    },
    {
        .ml_name = QUICK_SORT,
        .ml_meth = (PyCFunction) Sort_QuickSort,
        .ml_flags = METH_VARARGS | METH_KEYWORDS,
        .ml_doc = Sort_QuickSort_doc,
    },
    {
        .ml_name = QUICK_SORT_RANDOM,
        .ml_meth = (PyCFunction) Sort_QuickSortRandom,
        .ml_flags = METH_VARARGS | METH_KEYWORDS,
        .ml_doc = Sort_QuickSortRandom_doc,
    },
    {NULL, NULL, 0, NULL}  // Sentinel
};

static PyModuleDef sortmodule = {
    PyModuleDef_HEAD_INIT,
    .m_name = "algorithms.sort",
    .m_doc = "Sorting algorithms.",
    .m_size = -1,
    .m_methods = SortMethods,
};

PyMODINIT_FUNC
PyInit_sort(void)
{
    return PyModule_Create(&sortmodule);
}
