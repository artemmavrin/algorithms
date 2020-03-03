# Algorithms

Just some Python algorithms implemented in C and C++.

The goal of this project is to learn the [Python C API](https://docs.python.org/c-api/index.html) and [Python C/C++ extensions](https://docs.python.org/extending/extending.html).

All the algorithms herein are exposed in a Python package `algorithms` that can be installed directly from this GitHub repo:

```bash
pip install git+https://github.com/artemmavrin/algorithms.git
```

Example usage:

```python
>>> from algorithms.sort import heap_sort
>>> heap_sort([5, 3, 4, 2, 1])
[1, 2, 3, 4, 5]
>>> from algorithms.graph import bfs
>>> graph = {0: [1, 2], 1: [0, 3], 2: [1, 3], 3: [4]}
>>> for node in bfs(graph, 0):
...     print(node)
... 
0
1
2
3
4
```

**Note.**
To install the project for development, clone the project repository and run some `make` commands:

```bash
git clone https://github.com/artemmavrin/algorithms.git
cd algorithms
# Optional but recommended: create and activate new virtual environment
make install  # Install algorithms package
make test  # Run unit tests
```

**Note.**
The libraries in this package should compile successfully on macOS 10.15 with Apple clang version 11.0.0 and Python 3.8.2.
It has not been tested on other platforms.
