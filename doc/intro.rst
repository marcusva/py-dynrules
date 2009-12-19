Introduction
============

This is the documentation for the dynrules dynamic rules creation
package. dynrules is a small package that allows you to create
rule-based scripts for adaptive AI systems automatically. It uses the
Dynamic Scripting technique and algorithms as described by `Pieter Spronck
<http://ticc.uvt.nl/~pspronck/>`_ in his paper *Adaptive Game AI
with Dynamic Scripting*.

The focus of the package is on weighted ruleset management and script
generation, rather than weight adjustments and fitness evaluation. There
is only limited support for that kind of functionality and most of it
has to be implemented by the specific user code.

dynrules is not meant to be a fully functional AI package, but rather a
supportive system for creating your own adaptive AI quickly.

Prerequisites
=============

dynrules needs at least **Python 2.4** to run and was successfully tested with
the versions **2.4, 2.5, 2.6, 3.0** and **3.1**. It features both, a C and
pure Python implementation, letting you choose whatever suits you
more. By default, both implementations will be installed, but the C
implementation will be preferred by the dynrules internals.

In case you want to build the C implementation, you will need a C
compiler, which supports parts of the C99 standard, such as the `GCC
<http://www.gnu.org/software/gcc/>`_.

Installing dynrules
===================

You can either use the python way of installing the package using
distutils or the make command using the Makefile. Simply type ::

    $ python setup.py install

for the traditional python way or ::

    $ make install

for using the Makefile.

It must be said that the install target of the Makefile does not do
anything different from the python way. It simply calls 'python setup.py
install'.

Binary packages
===============

dynrules is not provided as binary package by the author. It might be
that someone else set up such a package for your wanted operating system
or distribution. Those packages are usually not supported by the author,
which means that installation problems or similar issues, which do not
target dynrules directly, should be escalated to the respective supplier
of that package.

Notes on bzr usage
==================

The bzr repository version of dynrules is not intended to be used in a
production environment. Interfaces may change from one checkin to
another, methods, classes or modules can be broken and so on. If you
want more reliable code, please refer to the official releases.
