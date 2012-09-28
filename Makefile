top_srcdir = `pwd`
PYTHON ?= python
SUBDIRS = $(top_srcdir)/src \
	$(top_srcdir)/lib \
	$(top_srcdir)/doc \
	$(top_srcdir)/examples \
	$(top_srcdir)/cplusplus \
	$(top_srcdir)/test

all: clean build

docs:
	@cd doc/ && make html
	@mv doc/build/html doc/
	@cd doc/ && rm -rf build
	@cd cplusplus/ && make docs

dist: docs
	@echo "Creating dist..."
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
			rm -f *~ *.cache *.core *.pyc; \
		fi \
	done

release: clean dist

buildall: clean
	@python2.4 setup.py build
	@python2.5 setup.py build
	@python2.6 setup.py build
	@python2.7 setup.py build
	@python3.1 setup.py build
	@python3.2 setup.py build

installall:
	@python2.4 setup.py install
	@python2.5 setup.py install
	@python2.6 setup.py install
	@python2.7 setup.py install
	@python3.1 setup.py install
	@python3.2 setup.py install

purge_installs:
	rm -rf /usr/local/include/python2.4/dynrules*
	rm -rf /usr/local/include/python2.5/dynrules*
	rm -rf /usr/local/include/python2.6/dynrules*
	rm -rf /usr/local/include/python2.7/dynrules*
	rm -rf /usr/local/include/python3.1/dynrules*
	rm -rf /usr/local/include/python3.2/dynrules*
	rm -rf /usr/local/lib/python2.4/site-packages/dynrules*
	rm -rf /usr/local/lib/python2.5/site-packages/dynrules*
	rm -rf /usr/local/lib/python2.6/site-packages/dynrules*
	rm -rf /usr/local/lib/python2.7/site-packages/dynrules*
	rm -rf /usr/local/lib/python3.1/site-packages/dynrules*
	rm -rf /usr/local/lib/python3.2/site-packages/dynrules*
