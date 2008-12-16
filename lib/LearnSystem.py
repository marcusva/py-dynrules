##
## dynrules - Python dynamic rules engine
##
## Authors: Marcus von Appen
##
## This file is distributed under the Public Domain.
##

try:
    import cStringIO as stringio
except ImportError:
    import sys
    if sys.version_info[0] >= 3:
        import io as stringio
    else:
        import StringIO as stringio
from random import uniform

from dynrules.Rule import Rule
from dynrules.RuleSet import RuleSet

class LearnSystem (object):
    """LearnSystem (ruleset) -> LearnSystem

    Creates a new LearnSystem using a specific RuleSet.

    The LearnSystem class takes care of creating new scripts based on a
    predefined RuleSet. It does not evaluate the scripts nor modifies
    the rules written to them.

    The procedure of creating scripts is done using three phases:

    * header creation
    * rule code creation
    * footer creation

    The header and footer are freely choosable. You can simple override
    or reassign the create_header() and create_footer() methods to let
    them return your required code.
    """
    def __init__ (self, ruleset):
        if not isinstance (ruleset, RuleSet):
            raise TypeError ("ruleset must be a RuleSet")
        self._ruleset = ruleset
        self._maxtries = 100

        # Maximum script size, not including the header and footer.
        self._maxscriptsize = 1024

    def _set_ruleset (self, ruleset):
        """L._set_ruleset (ruleset) -> None

        Sets the RuleSet to use by the LearnSystem.
        """
        if not isinstance (ruleset, RuleSet):
            raise TypeError ("ruleset must be a RuleSet")
        self._ruleset = ruleset

    def _set_maxtries (self, maxtries):
        """L._set_maxtries (maxtries) -> None

        Sets the maximum amount of tries for inserting a Rule into a script.

        Raises a ValueError, if maxtries is < 1.
        """
        if int (maxtries) < 1:
            raise ValueError ("maxtries must be > 0")
        self._maxtries = maxtries

    def _set_maxscriptsize (self, maxscriptsize):
        """L._set_maxscriptsize (maxscriptsize) -> None

        Sets the maximum scriptsize for rules to insert.

        Raises a ValueError, if maxscriptsize is < 1.
        """
        if int (maxscriptsize) < 1:
            raise ValueError ("maxscriptsize must be > 0")
        self._maxscriptsize = maxscriptsize

    def create_script (self, scriptfile, maxrules):
        """L.create_script (scriptfile, maxrules) -> None

        Creates a script from the available RuleSet.

        Creates a script from the available RuleSet using the passed
        script file. A maximum of maxrules rules will be written.
        
        scriptfile can be either a file object or filename.
        In case of a file object it is assumed to be writeable and won't
        be closed on leaving the function (but flushed).
        
        IMPORTANT for Python 3.x: The CLearnSystem implementation requires a
        file object under Python 3.0 - file names are not supported.
        """
        isopen = False
        filep = None
        if type (scriptfile) is file:
            isopen = True
            filep = scriptfile # Already open
        else:
            filep = open (scriptfile, "a")

        filep.write (self.create_header ())
        filep.write (self.create_rules (maxrules))
        filep.write (self.create_footer ())
        filep.flush ()

        if not isopen:
            filep.close ()
        
    def create_header (self):
        """L.create_header () -> str

        Creates the header for the script file.

        The default implementation does nothing.
        """
        # Nothing to do for now.
        pass

    def create_footer (self):
        """L._create_footer () -> str

        Creates the footer for the script file.

        The default implementation does nothing.
        """
        # Nothing to do for now.
        pass

    def create_rules (self, maxrules):
        """L.create_rules (maxrules) -> str

        Creates a rule list from the currently active RuleSet.

        Creates maxrules from the set RuleSet and passes their code back
        as string for the script file.

        Adapted from Pieter Spronck's algorithm as explained in
        Spronck et al: 2005, 'Adaptive Game AI with Dynamic Scripting'
        """
        if maxrules <= 0:
            raise ValueError ("maxrules must be greater than 0")
        
        weights = self._ruleset.weight
        buf = stringio.StringIO ()
        maxtries = self._maxtries
        maxscriptsize = self._maxscriptsize
        buflen = 0
        rules = self._ruleset.rules
        
        for i in xrange (maxrules):
            tries = 0
            while tries < maxtries:
                j = 0
                selected = -1
                wsum = 0
                fraction = uniform (0, weights)
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
                
                if buflen + len (rules[selected].code) > maxscriptsize:
                    return buf.getvalue ()
                
                buf.write (rules[selected].code)
                buflen = len (buf.getvalue ())
                
                if buflen >= maxscriptsize:
                    return buf.getvalue ()
                tries += 1
                break
        return buf.getvalue ()

    maxtries = property (lambda self: self._maxtries,
                         lambda self, var: self._set_maxtries (var),
                         doc = "Gets or sets the maximum amount of tries to" +\
                         "insert a script rule")
    maxscriptsize = property (lambda self: self._maxscriptsize,
                              lambda self, var: self._set_maxscriptsize (var),
                              doc = "Gets or sets the maximum script size " +\
                              "inserting rules")
    ruleset = property (lambda self: self._ruleset,
                        lambda self, var: self._set_ruleset (var),
                        doc = "Gets or sets the RuleSet to use")
