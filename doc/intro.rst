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
-------------
You must have at least one of the following Python versions installed:

* Python 2.6+           (http://www.python.org)
* PyPy 1.8.0+           (http://www.pypy.org)
* IronPython 2.7.2.1+   (http://www.ironpython.net)

Installing dynrules
-------------------
You can either use the python way of installing the package using
distutils or the make command using the Makefile. Simply type ::

    $ python setup.py install

for the traditional python way or ::

    $ make install

for using the Makefile.

It must be said that the install target of the Makefile does not do
anything different from the python way. It simply calls 'python setup.py
install'.

Trying out
^^^^^^^^^^
You also can test out dynrules without actually installing it. You just
need to set up your ``PYTHONPATH`` to point to the location of the
source distribution package. On Windows-based platforms, you might use
something like ::

   set PYTHONPATH=C:\path\to\dynrules\:$PYTHONPATH

to define the ``PYTHONPATH`` on a command shell. On Linux/Unix, use ::

   export PYTHONPATH=/path/to/dynrules:$PYTHONPATH

for bourne shell compatibles or ::

   setenv PYTHONPATH /path/to/dynrules:$PYTHONPATH

for C shell compatibles. You can omit the `:$PYTHONPATH``, if you did not use
it so far and if your environment settings do not define it.

.. note::

   If you are using IronPython, use ``IRONPYTHONPATH`` instead of
   ``PYTHONPATH``.


Binary packages
---------------
dynrules is not provided as binary package by the author. It might be
that someone else set up such a package for your wanted operating system
or distribution. Those packages are usually not supported by the author,
which means that installation problems or similar issues, which do not
target dynrules directly, should be escalated to the respective supplier
of that package.

Notes on Mercurial usage
------------------------
The Mercurial repository version of dynrules is not intended to be used
in a production environment. Interfaces may change from one checkin to
another, methods, classes or modules can be broken and so on. If you
want more reliable code, please refer to the official releases.
