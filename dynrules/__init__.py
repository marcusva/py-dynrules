"""
dynrules is a small Python module designed to generate scripts from
predefined rules and rulesets.
"""
from random import uniform
import os
import sys

try:
    import cStringIO as stringio
except ImportError:
    if sys.version_info[0] >= 3:
        import io as stringio
    else:
        import StringIO as stringio

if sys.version_info[0] >= 3:
    unicode = str

__version__ = "0.1.0"
__all__ = ["Rule", "RuleSet", "RuleManager", "LearnSystem", "MMapRuleManager"]


class Rule(object):
    """
    Rule is a simple class type that carries a weight indicator and
    arbitrary code data for usage in the dynamic script generation
    process.
    """
    def __init__(self, rid):
        """Creates a new, empty Rule."""
        self._id = rid
        self._weight = 0
        self._used = False
        self.code = None

    @property
    def id(self):
        """Gets the id of the Rule."""
        return self._id

    @property
    def weight(self):
        """Gets or sets the weight of the Rule."""
        return self._weight

    @weight.setter
    def weight(self, weight):
        """Gets or sets the weight of the rule.

        Raises a ValueError, if weight is < 0.
        """
        if float(weight) < 0:
            raise ValueError("weight must not be negative")
        self._weight = float(weight)

    @property
    def used(self):
        """Indicates whether the Rule was used or not."""
        return self._used

    @used.setter
    def used(self, used):
        """Sets whether the Rule was used or not."""
        self._used = bool(used)


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


class RuleManager(object):
    """
    The RuleManager class takes care of loading and saving rules from
    arbitrary data sources.
    The base is an abstract class, which's load_rules() method must be
    implemented according to the specific needs of the application.
    """
    def __init__(self, maxrules):
        """Creates a new, empty RuleManager object."""
        if maxrules < 0:
            raise ValueError("maxrules must be greater than 0")
        self._maxrules = maxrules

    @property
    def maxrules(self):
        """The maximum amount of rules to manage."""
        return self._maxrules

    def load_rules(self, maxrules=-1):
        """Loads rules from the underlying data source.

        Loads rules from the underlying data source and returns them as
        list. The maxrules argument defines the amount of rules to
        load. If it is smaller than 0, all existing rules should be
        returned.

        This must be implemented by inheriting classes.
        """
        raise NotImplementedError("method not implemented")

    def save_rules(self, rules):
        """Saves the passed rules to the underlying data source.

        Saves the passed rules to the underlying data source and returns
        True on success or False, if saving the rules was not possible.

        This must be implemented by inheriting classes.
        """
        raise NotImplementedError("method not implemented")

    def save_rules_hint_file(self, filename, learnsystem):
        """Saves a LearnSystem/RuleSet combination to a physical file.
        """
        learnsystem.create_script(filename, self._maxrules)


class MMapRuleManager(RuleManager):
    """A simple memory-mapped RuleManager implementation that does
    not load its rules from an external data source.

    It is an extremely useful class for testing rules and basic
    algorithms, but due to the in-memory management of all rules, it
    should not be used in a productive environment, especially if large
    rule sets have to be managed.

    By default, the MMapRuleManager will reserve enough memory for
    the rules to manage, when it is constructed. It will NOT fill the
    rules with useful values though. It is up to caller to use
    load_rules() afterwards and fill the returned Rule instances with
    the necessary data.
    """
    def __init__(self, maxrules):
        """Creates a new MMapRuleManager instance."""
        RuleManager.__init__(self, maxrules)
        self._rules = []
        append = self._rules.append

        for i in range(maxrules):
            append(Rule(i))

    def load_rules(self, maxrules=-1):
        """Returns the internally managed rules or a certain subset."""
        if maxrules != 0:
            return list(self._rules)
        return self._rules[:maxrules]

    def save_rules(self, rules):
        """This does nothing and will always return True."""
        return True


class LearnSystem(object):
    """The LearnSystem class takes care of creating new scripts based on a
    predefined RuleSet. It does not evaluate the scripts nor modifies
    the rules written to them.

    The procedure of creating scripts is done using three phases:

    * header creation
    * rule code creation
    * footer creation

    The header and footer are freely choosable. You can simply override
    or reassign the create_header() and create_footer() methods to let
    them return your required code.
    """
    def __init__(self, ruleset):
        """Creates a new LearnSystem using a specific RuleSet."""
        if not isinstance(ruleset, RuleSet):
            raise TypeError("ruleset must be a RuleSet")
        self._ruleset = ruleset
        self._maxtries = 100

        # Maximum script size, not including the header and footer.
        self._maxscriptsize = 1024

    @property
    def ruleset(self):
        """Gets or sets the RuleSet to use by the LeanrSystem."""
        return self._ruleset

    @ruleset.setter
    def ruleset(self, ruleset):
        """Sets the RuleSet to use by the LearnSystem."""
        if not isinstance(ruleset, RuleSet):
            raise TypeError("ruleset must be a RuleSet")
        self._ruleset = ruleset

    @property
    def maxtries(self):
        """Gets or sets the maximum amount of tries to insert a script
        rule.
        """
        return self._maxtries

    @maxtries.setter
    def maxtries(self, maxtries):
        """Sets the maximum amount of tries for inserting a Rule into a
        script.

        Raises a ValueError, if maxtries is < 1.
        """
        if int(maxtries) < 1:
            raise ValueError("maxtries must be > 0")
        self._maxtries = maxtries

    @property
    def maxscriptsize(self):
        """Gets or sets the maximum script size for inserting rules."""
        return self._maxscriptsize

    @maxscriptsize.setter
    def maxscriptsize(self, maxscriptsize):
        """Sets the maximum scriptsize for rules to insert.

        Raises a ValueError, if maxscriptsize is < 1.
        """
        if int(maxscriptsize) < 1:
            raise ValueError("maxscriptsize must be > 0")
        self._maxscriptsize = maxscriptsize

    def create_script(self, scriptfile, maxrules):
        """Creates a script from the available RuleSet.

        Creates a script from the available RuleSet using the passed
        script file. A maximum of maxrules rules will be written.

        scriptfile can be either a file object or filename.
        In case of a file object it is assumed to be writeable and won't
        be closed on leaving the function (but flushed).
        """
        isopen = False
        filep = None
        if type(scriptfile) in (str, unicode):
            if sys.version_info[0] >= 3:
                filep = stringio.open(scriptfile, "a")
            else:
                filep = open(scriptfile, "a")
        else:
            filep = scriptfile
            isopen = True

        filep.write(self.create_header())
        filep.write(self.create_rules(maxrules))
        filep.write(self.create_footer())
        filep.flush()

        if not isopen:
            filep.close()

    def create_header(self):
        """Creates the header for the script file.

        The default implementation does nothing.
        """
        # Nothing to do for now.
        pass

    def create_footer(self):
        """Creates the footer for the script file.

        The default implementation does nothing.
        """
        # Nothing to do for now.
        pass

    def create_rules(self, maxrules):
        """Creates a rule list from the currently active RuleSet.

        Creates maxrules from the set RuleSet and passes their code back
        as string for the script file.

        Adapted from Pieter Spronck's algorithm as explained in
        Spronck et al: 2005, 'Adaptive Game AI with Dynamic Scripting'
        """
        if maxrules <= 0:
            raise ValueError("maxrules must be greater than 0")

        weights = self._ruleset.weight
        buf = stringio.StringIO()
        maxtries = self._maxtries
        maxscriptsize = self._maxscriptsize
        buflen = 0
        rules = self._ruleset.rules

        for i in range(maxrules):
            tries = 0
            while tries < maxtries:
                j = 0
                selected = -1
                wsum = 0
                fraction = uniform(0, weights)
                while selected == -1:
                    wsum += rules[j].weight
                    if wsum > fraction:
                        selected = j
                        break
                    else:
                        j += 1

                # Here we differ from Spronck's algorithm, which
                # adds a lineadded = ... check. Instead we use
                # the break statement below.
                #
                # Additionally we limit the physical scriptsize using a
                # byte count
                #
                if buflen + len(rules[selected].code) > maxscriptsize:
                    return buf.getvalue()

                buf.write(rules[selected].code)
                buflen = len(buf.getvalue())

                if buflen >= maxscriptsize:
                    return buf.getvalue()
                tries += 1
                break
        return buf.getvalue()
