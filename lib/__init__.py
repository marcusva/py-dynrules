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

__version__ = "0.0.4"

try:
    from _dynrules import Rule as CRule
    from _dynrules import RuleSet as CRuleSet
    from _dynrules import LearnSystem as CLearnSystem
except ImportError:
    pass

from Rule import Rule
from RuleSet import RuleSet
from LearnSystem import LearnSystem
from RuleManagement import RuleManagement
from MMapRuleManagement import MMapRuleManagement
