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
    import StringIO as stringio
from random import uniform

from Rule import Rule
from RuleSet import RuleSet

class LearnSystem (object):
    """LearnSystem (ruleset) -> LearnSystem

    Creates a new LearnSystem
    """
    def __init__ (self, ruleset):
        if not isinstance (ruleset, RuleSet):
            raise TypeError ("ruleset must be a RuleSet")
        self.ruleset = ruleset
        self.maxtries = 100

        # Maximum script size, not including the header and footer.
        self.maxscriptsize = 1024

    def create_script (self, scriptfile, maxrules):
        """L.create_script (scriptfile, maxrules) -> None

        Creates a script from the available RuleSet.

        Creates a script from the available RuleSet using the passed
        script file. A maximum of maxrules rules will be written.
        
        scriptfile can be either a file object or filename.
        The file object is assumed to be writeable and won't be closed
        on leaving the function (but flushed).
        """
        isopen = False
        fp = None
        if type (scriptfile) is file:
            isopen = True
            fp = scriptfile # Already open
        else:
            fp = open (scriptfile, "a")

        fp.write (self.create_header ())
        fp.write (self.create_rules (limit))
        fp.write (self.create_footer ())
        fp.flush ()

        if not isopen:
            fp.close ()
        
    def create_header (self):
        """L.create_header () -> str

        Creates the header for the script file.
        """
        # Nothing to do for now.
        pass

    def create_footer (self):
        """L._create_footer () -> str

        Creates the footer for the script file.
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
        
        weights = self.ruleset.weight
        buf = stringio.StringIO ()
        added = 0
        maxtries = self.maxtries
        maxscriptsize = self.maxscriptsize
        buflen = 0
        rules = self.ruleset.rules
        
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
                buflen = len (buf)
                
                if buflen >= maxscriptsize:
                    return buf.getvalue ()
                tries += 1
                break
        return buf.getvalue ()
