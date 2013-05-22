#!/usr/bin/env python
from distutils.core import setup
import os

if __name__ == "__main__":
    fname = os.path.join(os.path.dirname(os.path.abspath(__file__)), "README.txt")
    readme = open(fname, "r")
    long_desc = readme.read()
    readme.close()

    setupdata = {
        "name":  "dynrules",
        "version": "0.0.16",
        "description": "Simple dynamic ruleset system for adaptive AI",
        "long_description": long_desc,
        "author": "Marcus von Appen",
        "author_email": "marcus@sysfault.org",
        "license": "Public Domain",
        "url": "http://code.google.com/p/python-dynrules/",
        "packages": ["dynrules",
                     "dynrules.examples",
                     "dynrules.test",
                     "dynrules.test.util",
                     ],
        "package_dir": {"dynrules.examples" : "examples"},
        "classifiers": [
            "Development Status :: 5 - Production/Stable",
            "Intended Audience :: Developers",
            "Intended Audience :: Science/Research",
            "License :: Public Domain",
            "Operating System :: OS Independent",
            "Programming Language :: Python",
            "Programming Language :: Python :: 2.6",
            "Programming Language :: Python :: 2.7",
            "Programming Language :: Python :: 3",
            "Programming Language :: Python :: 3.1",
            "Programming Language :: Python :: 3.2",
            "Programming Language :: Python :: 3.3",
            "Programming Language :: Python :: Implementation :: CPython",
            "Programming Language :: Python :: Implementation :: IronPython",
            "Programming Language :: Python :: Implementation :: PyPy",
            "Topic :: Scientific/Engineering :: Artificial Intelligence",
            "Topic :: Software Development :: Code Generators",
            "Topic :: Software Development :: Libraries :: Python Modules",
          ],
        }
    setup(**setupdata)
