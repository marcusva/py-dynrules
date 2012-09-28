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

    def _set_used(self, used):
        """Sets whether the Rule was used or not."""
        self._used = bool(used)

    def _set_weight(self, weight):
        """Sets the weight of the rule.

        Raises a ValueError, if weight is < 0.
        """
        if float(weight) < 0:
            raise ValueError("weight must not be negative")
        self._weight = float(weight)

    id = property(lambda self: self._id, doc="Gets the id of the Rule")
    weight = property(lambda self: self._weight,
                      lambda self, var: self._set_weight(var),
                      doc="Gets or sets the weight of the Rule")
    used = property(lambda self: self._used,
                    lambda self, var: self._set_used(var),
                    doc="Indicates whether the Rule was used or not")
