#define PY_SSIZE_T_CLEAN
#include <Python.h>

#include "debug.h"
#include "selection.h"


static PyObject *
Selection_MinMax(PyObject *self, PyObject *args)
{
    PyObject *obj;
    PyObject **sequence;
    Py_ssize_t size;
    PyObject *min_max = NULL;

    (void) self;  // Unused parameter

    // Parse positional arguments
    if (!PyArg_UnpackTuple(args, "min_max", 1, 1, &obj))
        return NULL;

    // Get the underlying sequence of Python objects
    if (PyList_Check(obj)) {
        Py_INCREF(obj);
        sequence = ((PyListObject *) obj)->ob_item;
        size = PyList_GET_SIZE(obj);
    } else if (PyTuple_Check(obj)) {
        Py_INCREF(obj);
        sequence = ((PyTupleObject *) obj)->ob_item;
        size = PyTuple_GET_SIZE(obj);
    } else {
        obj = PySequence_List(obj);
        if (!obj) return NULL;
        sequence = ((PyListObject *) obj)->ob_item;
        size = PyList_GET_SIZE(obj);
    }

    min_max = MinMax(sequence, size);
    Py_DECREF(obj);
    return min_max;  // Could be NULL
}

PyDoc_STRVAR(Selection_MinMax_doc,
"min_max(seq) -> tuple\n\n"
"Return the smallest and largest items in a sequence");

// List of functions exposed by the module
static PyMethodDef SelectionMethods[] = {
    {
        .ml_name = "min_max",
        .ml_meth = Selection_MinMax,
        .ml_flags = METH_VARARGS,
        .ml_doc = Selection_MinMax_doc,
    },
    {NULL, NULL, 0, NULL}  // Sentinel
};

static PyModuleDef selectionmodule = {
    PyModuleDef_HEAD_INIT,
    .m_name = "algorithms.selection",
    .m_doc = "Selection algorithms.",
    .m_size = -1,
    .m_methods = SelectionMethods,
};

PyMODINIT_FUNC
PyInit_selection(void)
{
    return PyModule_Create(&selectionmodule);
}
