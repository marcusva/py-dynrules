##
## dynrules - Python dynamic rules engine
##
## Authors: Marcus von Appen
##
## This file is distributed under the Public Domain.
##

from Rule import Rule
from RuleManagement import RuleManagement

class MMapRuleManagement (RuleManagement):
    """MMapRuleManagement (maxrules) -> MMapRuleManagement
    
    Creates a new MMapRuleManagement instance.
    
    A simple memory-mapped RuleManagement implementation that does not
    load its rules from an external data source.

    It is an extremely useful class for testing rules and basic algorithms, but
    due to the in-memory management of all rules, it should not be used in a
    productive environment, especially if large rule sets have to be managed.

    By default, the MMapRuleManagement will reserve enough memory for the rules
    to manage, when it is constructed. It will NOT fill the rules with useful
    values though. It is up to caller to use load_rules() afterwards and fill
    the returned Rule instances with the necessary data.
    """
    def __init__ (self, maxrules):
        RuleManagement.__init__ (self, maxrules)
        self._rules = []
        append = self._rules.append
        
        for i in xrange (maxrules):
            append (Rule (i))

    def load_rules (self, maxrules=-1):
        """M.load_rules (maxrules=-1) -> [ Rule, Rule ... ]

        Returns the internally managed rules or a certain subset.
        """
        if maxrules != 0:
            return list (self._rules)
        return self._rules[:maxrules]
    
    def save_rules (self, rules):
        """M.save_rules (rules) -> bool
        
        This does nothing and will always return True.
        """
        return True
