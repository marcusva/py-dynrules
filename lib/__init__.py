##
## dynrules - Python dynamic rules engine
##
## Authors: Marcus von Appen
##
## This file is distributed under the Public Domain.
##

try:
    from _dynrules import Rule
    from _dynrules import RuleSet
    from _dynrules import LearnSystem
except ImportError:
    # C Module not available
    import Rule
    import RuleSet
    import LearnSystem
