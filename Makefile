top_srcdir = `pwd`
PYTHON ?= python
SUBDIRS = $(top_srcdir)/src \
	 $(top_srcdir)/lib \
	 $(top_srcdir)/doc \
	 $(top_srcdir)/doc/examples \
	 $(top_srcdir)/test

all: clean build

docs:
	@cd doc/ && make html

dist: docs
	@echo "Creating dist..."
	@cp -rf doc/build/html doc/html
	@$(PYTHON) setup.py sdist

bdist:
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
	@rm -rf build dist

	@for dir in $(SUBDIRS); do \
		if test -f $$dir/Makefile; then \
			make -C $$dir clean; \
		else \
			cd $$dir; \
			echo "Cleaning up in $$dir..."; \
			rm -f *~ *.cache *.core; \
		fi \
	done

release: clean dist
