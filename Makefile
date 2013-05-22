PYTHON ?= python
top_srcdir = `pwd`
PYTHONPATH ?= $(top_srcdir)
SUBDIRS = \
	$(top_srcdir)/dynrules \
	$(top_srcdir)/dynrules/test \
	$(top_srcdir)/dynrules/test/util \
	$(top_srcdir)/doc \
	$(top_srcdir)/examples \
	$(top_srcdir)/cplusplus

INTERPRETERS = python2.7 python3.2 python3.3 pypy2.0

all: clean build

docs:
	@cd doc/ && make html
	@mv doc/build/html doc/
	@cd doc/ && rm -rf build
	@cd cplusplus/ && make docs

dist: docs
	@echo "Creating dist..."
	@$(PYTHON) setup.py sdist --format=zip
	@$(PYTHON) setup.py sdist --format=gztar

bdist: clean docs
	@echo "Creating bdist..."
	@$(PYTHON) setup.py bdist

build:
	@echo "Running build..."
	@$(PYTHON) setup.py build
	@echo "Build finished, invoke 'make install' to install."

install:
	@echo "Installing..."
	@$(PYTHON) setup.py build install

clean:
	@echo "Cleaning up in $(top_srcdir)/ ..."
	@rm -f *.cache *.core *~ MANIFEST
	@rm -rf build dist __pycache__

	@for dir in $(SUBDIRS); do \
		if test -f $$dir/Makefile; then \
			make -C $$dir clean; \
		else \
			cd $$dir; \
			echo "Cleaning up in $$dir..."; \
			rm -rf *~ *.cache *.core *.pyc __pycache__; \
		fi \
	done

release: clean dist

buildall: clean
	@for interp in $(INTERPRETERS); do \
		$$interp setup.py build; \
	done

installall:
	@for interp in $(INTERPRETERS); do \
		$$interp setup.py install; \
	done

testall:
	@for interp in $(INTERPRETERS); do \
		PYTHONPATH=$(PYTHONPATH) $$interp -B -m dynrules.test.util.runtests || true; \
	done

testpackage:
	@for interp in $(INTERPRETERS); do \
		$$interp -c "import dynrules.test; dynrules.test.run()" || true \
	done

purge_installs:
	@for interp in $(INTERPRETERS); do \
		rm -rf /usr/local/lib/$$interp/site-packages/dynrules*; \
	done