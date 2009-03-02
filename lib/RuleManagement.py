##
## dynrules - Python dynamic rules engine
##
## Authors: Marcus von Appen
##
## This file is distributed under the Public Domain.
##

import sys
if sys.version_info[0] >= 3:
    import io

class RuleManagement (object):
    """RuleManagement (maxrules) -> RuleManagement

    Creates a new, empty RuleManagement object.

    The RuleManagement class takes care of loading and saving rules from
    arbitrary data sources.
    The base is an abstract class, which's load_rules() method must be
    implemented according to the specific needs of the application.
    """
    def __init__ (self, maxrules):
        if maxrules < 0:
            raise ValueError ("maxrules must be greater than 0")
        self._maxrules = marules

    def load_rules (self, maxrules=-1):
        """R.load_rules (maxrules=-1) -> [ Rule, Rule ... ]

        Loads rules from the underlying data source.
        
        Loads rules from the underlying data source and returns them as list.
        The maxrules argument defines the amount of rules to load. If it is
        smaller than 0, all existing rules should be returned.
        
        This must be implemented by inheriting classes.
        """
        raise NotImplementedError ("method not implemented")
    
    def save_rules (self, rules):
        """R.save_rules (rules) -> bool
        
        Saves the passed rules to the underlying data source.
        
        Saves the passed rules to the underlying data source and returns True
        on success or False, if saving the rules was not possible.
        
        This must be implemented by inheriting classes.
        """
        raise NotImplementedError ("method not implemented")
        
    def save_rules_hint_file (self, prefix, suffix, learnsystem):
        """R.save_rules_hint_file (prefix, suffix, learnsystem) -> None
        
        Saves a LearnSystem/RuleSet combination to a physical file.
        The file can have a specific, unique name in the form:
        
        %prefix%_rules_%suffix%.py
        """
        filename = "%s_rules_%s.py" % (prefix, suffix)
        learnsystem.create_script (filename, self._maxrules)
            
    
    maxrules = property (lambda self: self._maxrules,
                         doc = "The maximum amount of rule to manage")
