.PHONY: clean dev help install test

help:
	@ echo "Commands:"
	@ echo "    make install		Install the package"
	@ echo "    make dev			Install the package for development"
	@ echo "    make test			Run unit tests"
	@ echo "    make clean 			Remove auxiliary files"

install:
	python -m pip install -e .

dev:
	python -m pip install -e .[dev]

test:
	coverage run -m pytest src
	coverage report --show-missing

clean:
	rm -f .coverage
	rm -rf .pytest_cache
	rm -rf build
	rm -rf src/*.egg-info
	rm -rf src/algorithms/__pycache__
