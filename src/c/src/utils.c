#include "debug.h"
#include "utils.h"

void Swap(PyObject **a, PyObject **b) {
    PyObject *temp = *a;
    *a = *b;
    *b = temp;
}
