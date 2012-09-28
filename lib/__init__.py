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

__version__ = "0.0.15"

try:
    from dynrules._dynrules import Rule as CRule
    from dynrules._dynrules import RuleSet as CRuleSet
    from dynrules._dynrules import LearnSystem as CLearnSystem
except ImportError:
    pass

from dynrules.Rule import Rule
from dynrules.RuleSet import RuleSet
from dynrules.LearnSystem import LearnSystem
from dynrules.RuleManagement import RuleManagement
from dynrules.MMapRuleManagement import MMapRuleManagement
