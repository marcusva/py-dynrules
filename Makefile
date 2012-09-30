top_srcdir = `pwd`
PYTHON ?= python
SUBDIRS = \
	$(top_srcdir)/src \
	$(top_srcdir)/dynrules \
	$(top_srcdir)/dynrules/test \
	$(top_srcdir)/dynrules/test/util \
	$(top_srcdir)/doc \
	$(top_srcdir)/examples \
	$(top_srcdir)/cplusplus

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
	@python2.6 setup.py build
	@python2.7 setup.py build
	@python3.1 setup.py build
	@python3.2 setup.py build
	@python3.3 setup.py build
	@pypy1.9 setup.py build

installall:
	@python2.6 setup.py build install
	@make clean
	@python2.7 setup.py build install
	@make clean
	@python3.1 setup.py build install
	@make clean
	@python3.2 setup.py build install
	@make clean
	@python3.3 setup.py build install
	@make clean
	@pypy1.9 setup.py build install
	@make clean

testall:
	@rm -rf dynrules/test/*.pyc
	@-PYTHONPATH=$(PYTHONPATH) python2.6 dynrules/test/util/runtests.py
	@rm -rf dynrules/test/*.pyc
	@-PYTHONPATH=$(PYTHONPATH) python2.7 dynrules/test/util/runtests.py
	@rm -rf dynrules/test/*.pyc
	@-PYTHONPATH=$(PYTHONPATH) python3.1 dynrules/test/util/runtests.py
	@rm -rf dynrules/test/*.pyc
	@-PYTHONPATH=$(PYTHONPATH) python3.2 dynrules/test/util/runtests.py
	@rm -rf dynrules/test/*.pyc
	@-PYTHONPATH=$(PYTHONPATH) python3.3 dynrules/test/util/runtests.py
	@rm -rf dynrules/test/*.pyc
	@-PYTHONPATH=$(PYTHONPATH) pypy1.9 dynrules/test/util/runtests.py
	@rm -rf dynrules/test/*.pyc

testpackage:
	@python2.6 -c "import dynrules.test; dynrules.test.run()"
	@python2.7 -c "import dynrules.test; dynrules.test.run()"
	@python3.1 -c "import dynrules.test; dynrules.test.run()"
	@python3.2 -c "import dynrules.test; dynrules.test.run()"
	@python3.3 -c "import dynrules.test; dynrules.test.run()"
	@pypy1.9 -c "import dynrules.test; dynrules.test.run()"

purge_installs:
	rm -rf /usr/local/include/python2.6/dynrules*
	rm -rf /usr/local/include/python2.7/dynrules*
	rm -rf /usr/local/include/python3.1/dynrules*
	rm -rf /usr/local/include/python3.2/dynrules*
	rm -rf /usr/local/lib/python2.6/site-packages/dynrules*
	rm -rf /usr/local/lib/python2.7/site-packages/dynrules*
	rm -rf /usr/local/lib/python3.1/site-packages/dynrules*
	rm -rf /usr/local/lib/python3.2/site-packages/dynrules*
	rm -rf /usr/local/lib/python3.3/site-packages/dynrules*
	rm -rf /usr/local/lib/pypy-1.9/site-packages/dynrules*
