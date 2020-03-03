.PHONY: clean debug debug-test help install test

help:
	@ echo "Commands:"
	@ echo "    make install		Install the package"
	@ echo "    make dev			Install the package for development"
	@ echo "    make debug			Install with debugging options enabled"
	@ echo "    make test			Run unit tests"
	@ echo "    make debug-test		Run unit tests in with debuggine turned on"
	@ echo "    make clean 			Remove auxiliary files"

install:
	python -m pip install -e .

debug: clean
	python -m pip install --global-option="--debug" -e .

test:
	python -m unittest discover --start-directory tests --verbose

debug-test: debug test

clean:
	rm -f .coverage
	rm -rf .pytest_cache
	rm -f src/python/algorithms/*.so
	rm -rf build
	rm -rf src/python/*.egg-info
