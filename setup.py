import os
import pathlib
import re
import sys
from distutils.core import Extension

from setuptools import setup, find_packages

if '--debug' in sys.argv:
    DEBUG = True
    sys.argv.remove('--debug')
else:
    DEBUG = False


def resolve_path(*parts):
    """Get a filename from a list of path components, relative to this file's
    parent directory."""
    return pathlib.Path(__file__).parent.joinpath(*parts).absolute()


def read_file(*parts):
    """Read a file's contents into a string."""
    filename = resolve_path(*parts)
    return filename.read_text()


__INIT__ = read_file('src', 'python', 'algorithms', '__init__.py')


def get_package_variable(name):
    pattern = rf'^{name} = [\'"](?P<value>[^\'"]*)[\'"]'
    match = re.search(pattern, __INIT__, flags=re.M)
    if match:
        return match.group('value')
    raise RuntimeError(f'Cannot find variable {name}')


COMPILER_FLAGS = ['-pedantic', '-Werror', '-Wall', '-Wextra',
                '-Wno-missing-field-initializers']
if DEBUG:
    COMPILER_FLAGS.extend(['-g', '-Og'])
    DEFINE_MACROS = [('DEBUG', None)]
    UNDEF_MACROS = ['NDEBUG']
else:
    COMPILER_FLAGS.append('-O2')
    DEFINE_MACROS = [('NDEBUG', None)]
    UNDEF_MACROS = []

C_INCLUDE_DIRS = ['src/c/include']
C_FLAGS = COMPILER_FLAGS + ['-std=c11']

CPP_INCLUDE_DIRS = ['src/cpp/include']
CPP_FLAGS = COMPILER_FLAGS + ['-std=c++11']

EXTENSION_KWARGS = dict(
    define_macros=DEFINE_MACROS,
    undef_macros=UNDEF_MACROS,
    optional=False,
)
C_EXTENSION_KWARGS = dict(
    extra_compile_args=C_FLAGS,
    include_dirs=C_INCLUDE_DIRS,
    **EXTENSION_KWARGS,
)
CPP_EXTENSION_KWARGS = dict(
    extra_compile_args=CPP_FLAGS,
    include_dirs=CPP_INCLUDE_DIRS,
    **EXTENSION_KWARGS,
)

EXT_MODULES = [
    Extension(
        name='algorithms.selection',
        sources=[
            'src/c/modules/selectionmodule.c',
            'src/c/src/selection.c',
            'src/c/src/utils.c',
        ],
        **C_EXTENSION_KWARGS,
    ),
    Extension(
        name='algorithms.sort',
        sources=[
            'src/c/modules/sortmodule.c',
            'src/c/src/heap.c',
            'src/c/src/partition.c',
            'src/c/src/search.c',
            'src/c/src/sort.c',
            'src/c/src/utils.c',
        ],
        **C_EXTENSION_KWARGS,
    ),
    Extension(
        name='algorithms.graph',
        sources=[
            'src/cpp/modules/graphmodule.cpp',
        ],
        **CPP_EXTENSION_KWARGS,
    ),
    Extension(
        name='algorithms.random',
        sources=[
            'src/cpp/modules/randommodule.cpp',
            'src/cpp/src/random.cpp',
        ],
        **CPP_EXTENSION_KWARGS,
    ),
]

setup(
    name=get_package_variable('__package__'),
    version=get_package_variable('__version__'),
    description=get_package_variable('__description__'),
    url=get_package_variable('__url__'),
    author=get_package_variable('__author__'),
    author_email=get_package_variable('__author_email__'),
    long_description=read_file('README.md'),
    long_description_content_type='text/markdown',
    packages=find_packages(os.path.join('src', 'python')),
    package_dir={'': os.path.join('src', 'python')},
    ext_modules=EXT_MODULES,
    classifiers=[
        'Development Status :: 1 - Planning',
        'Intended Audience :: Developers',
        'Intended Audience :: Education',
        'Operating System :: OS Independent',
        'Programming Language :: Python :: 3.8',
    ],
    zip_safe=False,
)
