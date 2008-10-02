##
## dynrules - Python dynamic rules engine
##
## Authors: Marcus von Appen
##
## This file is distributed under the Public Domain.
##

class Rule (object):
    def __init__ (self, rid):
        self.id = rid
        self.weight = 0
        self.code = None
        self.used = False
