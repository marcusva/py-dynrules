top_srcdir = `pwd`

SUBDIRS = $(top_srcdir)/src \
	 $(top_srcdir)/lib \
	 $(top_srcdir)/doc \
	 $(top_srcdir)/doc/examples \
	 $(top_srcdir)/test

all: clean build

dist:
	@echo "Creating dist..."
	@python setup.py sdist

bdist:
	@echo "Creating bdist..."
	@python setup.py bdist

build:
	@echo "Running build..."
	@python setup.py build
	@echo "Build finished, invoke 'make install' to install."

install:
	@echo "Installing..."
	@python setup.py build install 

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
