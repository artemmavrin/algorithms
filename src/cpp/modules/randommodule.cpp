#define PY_SSIZE_T_CLEAN  // Make "s#" use Py_ssize_t rather than int
#include <Python.h>

#include "random.h"

static PyObject *
Random_Sample(PyObject *self, PyObject *args, PyObject *kwargs)
{
    PyObject *population;
    PyObject *seed = nullptr;
    Py_ssize_t sample_size = 1;

    PyObject *sample = nullptr;  // Guilty until proven innocent

    (void) self;  // Unused parameter

    // Parse positional arguments
    static const char *format = "O|nO";
    static const char *keywords[] = {"", "size", "seed", nullptr};
    if (!PyArg_ParseTupleAndKeywords(args, kwargs, format, (char **) keywords,
                                     &population, &sample_size, &seed))
        return nullptr;

    // Sample size must be non-negative
    if (sample_size < 0) {
        PyErr_SetString(PyExc_ValueError, "sample size must be non-negative");
        return nullptr;
    }

    // Population must be iterable
    if (!(population = PyObject_GetIter(population))) return nullptr;

    sample = Sample(population, sample_size, seed);
    Py_DECREF(population);
    return sample;  // Might be NULL
}

PyDoc_STRVAR(Random_Sample_doc,
"sample(population, size=1, seed=None) -> list\n\n"
"Draw a random sample without replacement from a finite population.\n\n"
"The iterable `population` must be finite, but its length need not be known\n"
"in advance since the sampling is done in one pass. The sampling algorithm is\n"
"Algorithm R in Jeffrey S. Vitter, \"Random sampling with a reservoir\". ACM\n"
"Transactions on Mathematical Software. Volume 11, Issue 1 (1985), pp. 37--57\n"
"DOI: https://doi.org/10.1145%2F3147.3165");

// List of functions exposed by the module
static PyMethodDef RandomMethods[] = {
    {
        "sample",                       // ml_name
        (PyCFunction) Random_Sample,    // ml_meth
        METH_VARARGS | METH_KEYWORDS,   // ml_flags
        Random_Sample_doc,              // ml_doc
    },
    {NULL, NULL, 0, NULL}  // Sentinel
};

static PyModuleDef randommodule = {
    PyModuleDef_HEAD_INIT,
    "algorithms.random",                // m_name
    "Randomized algorithms.",           // m_doc
    -1,                                 // m_size
    RandomMethods,                      // m_methods
};

PyMODINIT_FUNC
PyInit_random(void)
{
    return PyModule_Create(&randommodule);
}
