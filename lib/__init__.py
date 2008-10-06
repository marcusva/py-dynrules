##
## dynrules - Python dynamic rules engine
##
## Authors: Marcus von Appen
##
## This file is distributed under the Public Domain.
##

__version__ = "0.0.1"

try:
    from _dynrules import Rule
    from _dynrules import RuleSet
    from _dynrules import LearnSystem
except ImportError:
    # C Module not available
    from Rule import Rule
    from RuleSet import RuleSet
    from LearnSystem import LearnSystem

