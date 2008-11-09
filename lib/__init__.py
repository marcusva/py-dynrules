##
## dynrules - Python dynamic rules engine
##
## Authors: Marcus von Appen
##
## This file is distributed under the Public Domain.
##

"""dynrules is a small Python module designed to generate scripts from
predefined rules and rulesets.
"""

__version__ = "0.0.3"

try:
    from _dynrules import Rule
    from _dynrules import RuleSet
    from _dynrules import LearnSystem
except ImportError:
    # C Module not available
    from Rule import Rule
    from RuleSet import RuleSet
    from LearnSystem import LearnSystem

