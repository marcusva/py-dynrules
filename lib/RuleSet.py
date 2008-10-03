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
        self._minweight = minweight
        self._maxweight = maxweight
        self._weight = 0

    def _set_minweight (self, minweight):
        """R._set_minweight (minweight) -> None

        Sets the minimum weight to use for Rules.

        Raises a ValueError, if minweight is negative or greater than
        maxweight.
        """
        val = float (minweight)
        if val < 0:
            raise ValueError ("minweight must not be negative")
        if val > self._maxweight:
            raise ValueError \
                  ("minweight must be smaller or equal to the set maxweight")
        self._minweight = minweight

    def _set_maxweight (self, maxweight):
        """R._set_maxweight (maxweight) -> None

        Sets the maximum weight to use for Rules.

        Raises a ValueError, if maxweight is negative or smaller than
        minweight.
        """
        val = float (maxweight)
        if val < 0:
            raise ValueError ("maxweight must not be negative")
        if val < self._minweight:
            raise ValueError \
                  ("maxweight must be smaller or equal to the set minweight")
        self._maxweight = maxweight

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

        Removes a Rule from the RuleSet.
        """
        if not isinstance (rule, Rule):
            raise TypeError ("rule must be a Rule")
        if not self.rules.has_key (rule.id):
            raise ValueError ("rule does not exist")
        
        rule = self.rules[rule.id]
        self._weight -= rule.weight
        del self.rules[rule.id]

    def calculate_adjustment (self, fitness):
        """R.calculate_adjustment (fitness)

        Calculates the reward or penalty for the active rules.

        Calculates the reward or penalty, each of the activated rules
        recives. fitness hereby can be used as measure of the
        performance or whatever is suitable in the implementation.

        This must be implemented by inheriting classes.
        """
        raise NotImplementedError ("method not implemented")

    def distribute_remainder (self, remainder):
        """R.distribute_remainder (remainder) -> alue

        Distributes the remainder of the weight differences.

        Distributes the remainder of the weight differences between the
        last weights and current weights.

        The method must return a value
        
        This must be implemented by inheriting classes.
        """
        raise NotImplementedError ("method not implemented")
        
    def update_weights (self, fitness):
        """
        Adapted from Pieter Spronck's algorithm as explained in
        Spronck et al: 2005, 'Adaptive Game AI with Dynamic Scripting'
        """
        used = 0
        rules = self.rules.values ()
        count = len (rules)

        usedcount = 0
        for r in rules:
            if r.used:
                usedcount += 1
        minweight = self.minweight
        maxweight = self.maxweight
        totweight = 0

        if usedcount == 0 or usedcount == count:
            return

        nonactive = count - usedcount
        adjustment = self.calculate_adjustment (fitness)
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

        self._weight = totweight
        
        self.distribute_remainder (remainder)
        totweight = 0
        for rule in rules:
            totweight += rule.weight
        self._weight = totweight

    rules = property (lambda self: self._rules.values (), doc = "")
    minweight = property (lambda self: self._minweight,
                    lambda self, var: self._set_minweight (var),
                    doc = "Gets or sets the minimum weight to use for Rules")
    maxweight = property (lambda self: self._maxweight,
                    lambda self, var: self._set_maxweight (var),
                    doc = "Gets or sets the maximum weight to use for Rules")
    weight = property (lambda self: self._weight,
                       doc = "Gets the total weight of all Rules")
