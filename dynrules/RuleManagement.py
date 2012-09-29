##
## dynrules - Python dynamic rules engine
##
## Authors: Marcus von Appen
##
## This file is distributed under the Public Domain.
##


class RuleManagement(object):
    """
    The RuleManagement class takes care of loading and saving rules from
    arbitrary data sources.
    The base is an abstract class, which's load_rules() method must be
    implemented according to the specific needs of the application.
    """
    def __init__(self, maxrules):
        """Creates a new, empty RuleManagement object."""
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
