#include "random.h"

// The only reason this is C++ and not C is the better random number generators
// and functions in the C++ standard library
#include <random>

// Draw a random sample without replacement from the iterable `population` of
// length `n` in `O(n)` steps (i.e., independent of `sample_size`).
PyObject *
Sample(PyObject *population, Py_ssize_t sample_size, PyObject *seed) {
    PyObject *reservoir = nullptr;
    Py_ssize_t i;
    std::mt19937 rng;

    // Allocate memory for the reservoir
    if (!(reservoir = PyList_New(sample_size))) return nullptr;

    // Put an initial segment of the population into the new list
    for (i = 0; i < sample_size; ++i) {
        PyObject *item = PyIter_Next(population);
        if (PyErr_Occurred()) {
            Py_DECREF(reservoir);
            return nullptr;
        } else if (!item) {
            PyErr_SetString(PyExc_ValueError, "population is too small");
            Py_DECREF(reservoir);
            return nullptr;
        }
        PyList_SET_ITEM(reservoir, i, item);
    }

    // Seed the random number generator
    if (seed == nullptr) {
        rng.seed(std::random_device()());
    } else {
        unsigned long seed_ = PyLong_AsUnsignedLong(seed);
        if (PyErr_Occurred()) {
            Py_DECREF(reservoir);
            return nullptr;
        }
        rng.seed(seed_);
    }

    // Iterate over the remaining items in the population, randomly replacing
    // items in the reservoir with the later items in the population
    for (;; ++i) {
        PyObject *item = PyIter_Next(population);
        if (PyErr_Occurred()) {
            Py_DECREF(reservoir);
            return nullptr;
        } else if (!item) {
            // Population exhausted
            break;
        }
        std::uniform_int_distribution<Py_ssize_t> randint(0, i);
        Py_ssize_t j = randint(rng);
        if (j < sample_size) {
            PyList_SET_ITEM(reservoir, j, item);
        }
    }

    return reservoir;
}
