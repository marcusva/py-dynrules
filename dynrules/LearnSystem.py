##
## dynrules - Python dynamic rules engine
##
## Authors: Marcus von Appen
##
## This file is distributed under the Public Domain.
##
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

from dynrules.RuleSet import RuleSet


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
