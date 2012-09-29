##
## dynrules - Python dynamic rules engine
##
## Authors: Marcus von Appen
##
## This file is distributed under the Public Domain.
##


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
