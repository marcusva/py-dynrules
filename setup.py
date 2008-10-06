#!/usr/bin/env python
import distutils.sysconfig
from distutils.core import setup, Extension
import os, sys, glob

DEBUG = "1"
VERSION = "0.0.1"

def get_sources ():
    return glob.glob (os.path.join ("src", "*.c"))

def get_headers ():
    return [ "src/compat.h", "src/dynrules.h" ]

def get_defines ():
    defines = [("VERSION", "\"0.0.1\"")]
    return defines

##
# General configuration stuff.
##
if __name__ == "__main__":

    compile_args = []

    # GCC specific code START.
##     if DEBUG != "0":
##         compile_args += ["-W", "-Wall", "-Wpointer-arith", "-Wcast-qual",
##                       "-Winline", "-Wcast-align", "-Wconversion",
##                       "-Wstrict-prototypes", "-Wmissing-prototypes",
##                       "-Wmissing-declarations", "-Wnested-externs",
##                       "-Wshadow", "-Wredundant-decls"
##                      ]
##     compile_args += ["-std=c99", "-g"]
    # GCC specific code END.
    
    dynrules = Extension ("dynrules._dynrules", sources=get_sources (),
                          language="c",define_macros=get_defines (),
                          extra_compile_args=compile_args)

    setupdata = {
        "name" :  "dynrules",
        "version" : VERSION,
        "description": "Simple dynamic ruleset system",
        "author": "Marcus von Appen",
        "author_email": "marcus@sysfault.org",
        "license": "Public Domain",
        "url": "http://www.sysfault.org/projects/",
        "packages" : ["dynrules"],
        "package_dir" : { "dynrules" : "lib" },
        "ext_modules" : [ dynrules ],
        "headers" : get_headers (),
        }
    setup (**setupdata)
