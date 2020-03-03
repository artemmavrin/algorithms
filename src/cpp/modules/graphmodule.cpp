#define PY_SSIZE_T_CLEAN  // Make "s#" use Py_ssize_t rather than int
#include "Python.h"

// Graph algorithms

#include <queue>

// Breath-first search traversal of a directed graph, represented as an
// adjacency list

typedef struct {
    PyObject_HEAD
    PyObject *graph;
    PyObject *visited;
    std::queue<PyObject *> *queue;
} BFSObject;

// Instantiate a new BFSObject from a graph and a distinguished root node
static PyObject *
BFS_New(PyTypeObject *type, PyObject *args, PyObject *kwargs)
{
    PyObject *graph;
    PyObject *root;
    PyObject *visited = nullptr;
    std::queue<PyObject *> *queue = nullptr;
    BFSObject *bfs = nullptr;

    // Parse arguments
    static const char *format = "OO";
    static const char *keywords[] = {"graph", "root", nullptr};
    if (!PyArg_ParseTupleAndKeywords(args, kwargs, format, (char **) keywords,
                                     &graph, &root))
        return nullptr;

    // The graph must support the mapping protocol (it should be the adjacency
    // list representation of a graph)
    if (!PyMapping_Check(graph)) {
        PyErr_SetString(PyExc_TypeError, "bfs() expects a mapping type");
        return nullptr;
    }

    // Allocate memory needed for the traversal
    if (!(queue = new(std::nothrow) std::queue<PyObject *>())) goto fail;
    if (!(visited = PySet_New(nullptr))) goto fail;
    if (!(bfs = (BFSObject *) type->tp_alloc(type, 0))) goto fail;

    // Finalize the creation of the new BFS object
    Py_INCREF(graph);
    bfs->graph = graph;
    bfs->visited = visited;
    bfs->queue = queue;

    // Mark the root node as visited and put it in the queue
    // We mark nodes as visited while they're still in the queue to avoid
    // putting duplicate nodes in the queue. This way we don't have to check if
    // a node has been visited when we pop from the queue, and we save on some
    // queue memory
    Py_INCREF(root);
    if (PySet_Add(bfs->visited, root) < 0) {
        Py_DECREF(root);
        goto fail;
    }
    Py_INCREF(root);
    bfs->queue->push(root);

    return (PyObject *) bfs;

fail:
    // Avoid memory leaks on failure
    if (visited) Py_DECREF(visited);
    if (queue) delete queue;
    return nullptr;
}

// Deallocate all memory and destroy all references associated with a BFSObject
static void
BFS_Dealloc(PyObject *self)
{
    BFSObject *bfs = (BFSObject *) self;
    Py_DECREF(bfs->graph);
    Py_DECREF(bfs->visited);
    while (!(bfs->queue->empty())) {
        PyObject *node = bfs->queue->front();
        Py_DECREF(node);
        bfs->queue->pop();
    }
    delete bfs->queue;
    Py_TYPE(bfs)->tp_free(bfs);
}

// Get the next node in the traversal, or NULL if there are no more nodes
static PyObject *
BFS_Next(PyObject *self)
{
    BFSObject *bfs = (BFSObject *) self;
    PyObject *node, *neighbors, *neighbors_it;
    int status;

    if (bfs->queue->empty()) return nullptr;  // Traversal has finished

    // Nodes in the queue are guaranteed to be visited exactly once
    node = bfs->queue->front();
    bfs->queue->pop();

    // Get the neighbors of the current node
    if (!(neighbors = PyObject_GetItem(bfs->graph, node))) {
        Py_DECREF(node);
        if (PyErr_ExceptionMatches(PyExc_KeyError)) {
            // There is no adjacency list for the given node, which we interpret
            // as the node having zero outgoing edges
            PyErr_Clear();
            return node;
        }
        return nullptr;
    }

    // The neighbors object must be iterable
    if (!(neighbors_it = PyObject_GetIter(neighbors))) {
        Py_DECREF(node);
        Py_DECREF(neighbors);
        return nullptr;
    }
    Py_DECREF(neighbors);

    // Add the unvisited neighbors of the current node to the queue
    while (PyObject *neighbor = PyIter_Next(neighbors_it)) {
        if ((status = PySet_Contains(bfs->visited, neighbor))) {
            Py_DECREF(neighbor);
            if (status < 0) {
                // The containment check failed
                Py_DECREF(node);
                Py_DECREF(neighbors_it);
                return nullptr;
            }
            // The current neighbor has already been visited
            continue;
        }

        // Mark this neighbor as visited
        if (PySet_Add(bfs->visited, neighbor) < 0) {
            Py_DECREF(node);
            Py_DECREF(neighbor);
            Py_DECREF(neighbors_it);
            return nullptr;
        }
        Py_INCREF(neighbor);
        bfs->queue->push(neighbor);
    }
    if (PyErr_Occurred()) {
        Py_DECREF(node);
        Py_DECREF(neighbors_it);
        return nullptr;
    }
    Py_DECREF(neighbors_it);

    return node;
}

PyDoc_STRVAR(BFS_Type_doc,
"bfs(graph, root)\n\n"
"Breadth-first traversal of a directed graph, starting at root.\n\n"
"The graph should be represented as an adjacency list (e.g., the graph\n"
"0 -> 1 -> 2 is represented as the dict {0: [1], 1: [2]}). Note that we don't\n"
"include 2 as a key in the dictionary; this is interpreted as 2 having no\n"
"outgoing edges.\n\n"
">>> graph = {0: [1, 2], 1: [3, 4], 2: [5, 6], 4: [4, 5, 6, 0]}\n"
">>> b = bfs(graph, 0)\n"
">>> list(b)\n"
"[0, 1, 2, 3, 4, 5, 6]");

static PyTypeObject BFS_Type = {
    PyVarObject_HEAD_INIT(&PyType_Type, 0)
    "bfs",                              /* tp_name */
    sizeof(BFSObject),                  /* tp_basicsize */
    0,                                  /* tp_itemsize */
    BFS_Dealloc,                        /* tp_dealloc */
    0,                                  /* tp_print */
    0,                                  /* tp_getattr */
    0,                                  /* tp_setattr */
    0,                                  /* tp_reserved */
    0,                                  /* tp_repr */
    0,                                  /* tp_as_number */
    0,                                  /* tp_as_sequence */
    0,                                  /* tp_as_mapping */
    0,                                  /* tp_hash */
    0,                                  /* tp_call */
    0,                                  /* tp_str */
    0,                                  /* tp_getattro */
    0,                                  /* tp_setattro */
    0,                                  /* tp_as_buffer */
    Py_TPFLAGS_DEFAULT,                 /* tp_flags */
    BFS_Type_doc,                       /* tp_doc */
    0,                                  /* tp_traverse */
    0,                                  /* tp_clear */
    0,                                  /* tp_richcompare */
    0,                                  /* tp_weaklistoffset */
    PyObject_SelfIter,                  /* tp_iter */
    BFS_Next,                           /* tp_iternext */
    0,                                  /* tp_methods */
    0,                                  /* tp_members */
    0,                                  /* tp_getset */
    0,                                  /* tp_base */
    0,                                  /* tp_dict */
    0,                                  /* tp_descr_get */
    0,                                  /* tp_descr_set */
    0,                                  /* tp_dictoffset */
    0,                                  /* tp_init */
    PyType_GenericAlloc,                /* tp_alloc */
    BFS_New,                            /* tp_new */
};

static struct PyModuleDef graphmodule = {
   PyModuleDef_HEAD_INIT,
   "algorithms.graph",                  /* m_name */
   "Graph algorithms.",                 /* m_doc */
   -1,                                  /* m_size */
};

PyMODINIT_FUNC
PyInit_graph(void)
{
    PyObject *module = PyModule_Create(&graphmodule);
    if (!module)
        return NULL;

    if (PyType_Ready(&BFS_Type) < 0)
        return NULL;
    Py_INCREF(&BFS_Type);
    PyModule_AddObject(module, "bfs", (PyObject *) &BFS_Type);

    return module;
}
