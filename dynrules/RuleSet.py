##
## dynrules - Python dynamic rules engine
##
## Authors: Marcus von Appen
##
## This file is distributed under the Public Domain.
##
from dynrules.Rule import Rule


class RuleSet(object):
    """
    RuleSet is a rule container class that manages rules, their weights
    and the weight distribution for the rules. The minweight and maxweight
    parameters are the minimum and maximum weight boundaries, each rule's
    weight has to stay in.
    """
    def __init__(self, minweight, maxweight):
        """Creates a new, empty RuleSet."""
        self._rules = {}
        self._weight = 0
        self._maxweight = 0
        self._minweight = 0
        self.maxweight = maxweight
        self.minweight = minweight

    @property
    def rules(self):
        """Gets the list of currently managed Rule objects."""
        return list(self._rules.values())

    @property
    def weight(self):
        """Gets the total weight of all managed Rules."""
        return self._weight

    @property
    def minweight(self):
        """Gets or sets the minimum weight to use for rules."""
        return self._minweight

    @minweight.setter
    def minweight(self, minweight):
        """Sets the minimum weight to use for Rules.

        Raises a ValueError, if minweight is negative or greater than
        maxweight.
        """
        val = float(minweight)
        if val < 0:
            raise ValueError("minweight must not be negative")
        if val > self._maxweight:
            raise ValueError \
                  ("minweight must be smaller or equal to the set maxweight")
        self._minweight = val

    @property
    def maxweight(self):
        """Gets or sets the maximum weight to use for rules."""
        return self._maxweight

    @maxweight.setter
    def maxweight(self, maxweight):
        """Sets the maximum weight to use for Rules.

        Raises a ValueError, if maxweight is negative or smaller than
        minweight.
        """
        val = float(maxweight)
        if val < 0:
            raise ValueError("maxweight must not be negative")
        if val < self._minweight:
            raise ValueError \
                  ("maxweight must be smaller or equal to the set minweight")
        self._maxweight = val

    def clear(self):
        """Removes all rules from the RuleSet."""
        self._rules.clear()
        self._weight = 0

    def add(self, rule):
        """Adds a Rule to the RuleSet."""
        if not isinstance(rule, Rule):
            raise TypeError("rule must be a Rule")
        if rule.id in self._rules:
            self._weight -= self._rules[rule.id].weight

        self._rules[rule.id] = rule
        if rule.weight > self.maxweight:
            rule.weight = self.maxweight
        elif rule.weight < self.minweight:
            rule.weight = self.minweight
        self._weight += rule.weight

    def find(self, rid):
        """Tries to find the Rule with the matching id and returns it.

        In case no Rule with the passed id exists, None is returned.
        """
        if rid in self._rules:
            return self._rules[rid]
        return None

    def remove(self, rule):
        """Removes a Rule from the RuleSet."""
        if not isinstance(rule, Rule):
            raise TypeError("rule must be a Rule")
        if not rule.id in self._rules:
            raise ValueError("rule does not exist")
        if rule != self._rules[rule.id]:
            raise ValueError("rule does not match rule in RuleSet")

        self._weight -= self._rules[rule.id].weight
        del self._rules[rule.id]

    def calculate_adjustment(self, fitness):
        """Calculates the reward or penalty for the active rules.

        Calculates the reward or penalty, each of the activated rules
        recives. fitness hereby can be used as measure of the
        performance or whatever is suitable in the implementation.

        This must be implemented by inheriting classes.
        """
        raise NotImplementedError("method not implemented")

    def distribute_remainder(self, remainder):
        """Distributes the remainder of the weight differences.

        Distributes the remainder of the weight differences between the
        last weights and current weights.

        The method must return a value.

        This must be implemented by inheriting classes.
        """
        raise NotImplementedError("method not implemented")

    def update_weights(self, fitness):
        """Updates the weights of all contained rules.

        Adapted from Pieter Spronck's algorithm as explained in
        Spronck et al: 2005, 'Adaptive Game AI with Dynamic Scripting'
        """
        rules = self._rules.values()
        count = len(rules)

        usedcount = 0
        for rule in rules:
            if rule.used:
                usedcount += 1
        minweight = self.minweight
        maxweight = self.maxweight
        totweight = 0

        if usedcount == 0 or usedcount == count:
            return

        nonactive = count - usedcount
        adjustment = self.calculate_adjustment(fitness)
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

        self.distribute_remainder(remainder)
        totweight = 0
        for rule in rules:
            totweight += rule.weight
        self._weight = totweight
