##
## dynrules - Python dynamic rules engine
##
## Authors: Marcus von Appen
##
## This file is distributed under the Public Domain.
##

from Rule import Rule

class RuleSet (object):
    """RuleSet () -> RuleSet

    Creates a new, empty RuleSet
    """
    def __init__ (self, minweight, maxweight):
        self._rules = {}
        self.minweight = minweight
        self.maxweight = maxweight
        self.weight = 0;

    def add (self, rule):
        """R.add (rule) -> None

        Adds a Rule to the RuleSet
        """
        if not isinstance (rule, Rule):
            raise TypeError ("rule must be a Rule")
        self.rules[rule.id] = rule
        
        if rule.weight > self.maxweight:
            rule.weight = self.maxweight
        elif rule.weight < self.minweight:
            rule.weight = self.minweight

    def remove (self, rule):
        """R.remove (rule) -> None

        Removes a Rule from the RuleSet
        """
        if not isinstance (rule, Rule):
            raise TypeError ("rule must be a Rule")
        del self.rules[rule.id]

    def update_weights (self):
        """
        Adapted from Pieter Spronck's algorithm as explained in
        Spronck et al: 2005, 'Adaptive Game AI with Dynamic Scripting'
        """
        used = 0
        rules = self.rules.values ()
        count = len (rules)
        usedrules = [x for x in rules if x.used]
        usedcount = len (usedrules)
        minweight = self.minweight
        maxweight = self.maxweight
        totweight = 0

        if usedcount == 0 or usedcount == count:
            return

        nonactive = count - usedcount
        adjustment = 0 # TODO: fitness invocation, blablabla
        compensation = - usedcount * adjustment / nonactive
        remainder = 0

        for rule in rules:
            if rule.used:
                rule.weight += adjustment
            else:
                rule.weight += compensation

            if rule.weight < minweight:
                remainder += rule.weight - minweight
                rule.weight = minweight
            elif rule.weight > maxweight:
                remainder += rule.weight - maxweight
                rule.weight = maxweight
            totweight += rule.weight

        # TODO: Distribute the difference remainder
        
        self.weight = totweight

    rules = property (lambda self: self._rules.values (), doc = "")
