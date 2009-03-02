:class:`CRuleSet` - Rule container
==================================
.. class:: CRuleSet (minweight, maxweight) -> CRuleSet

  Creates a new, empty :class:`CRuleSet`.
  
  :class:`CRuleSet` is a rule container class that manages rules, their
  weights and the weight distribution for the rules. The *minweight* and
  *maxweight* parameters are the minimum and maximum weight boundaries,
  each rule's weight has to stay in.

Attributes
----------

.. attribute:: CRuleSet.maxweight

  Gets or sets the maximum weight to use for rules.
  
.. attribute:: CRuleSet.minweight

  Gets or sets the minimum weight to use for rules.

.. attribute:: CRuleSet.rules

  Gets the list of currently managed :class:`CRule` objects.

.. attribute:: CRuleSet.weight

  Gets the total weight of all managed :class:`CRule` objects.

Methods
-------

.. method:: CRuleSet.add (rule) -> None

  Adds a :class:`CRule to the `:class:`CRuleSet`.

.. method:: CRuleSet.calculate_adjustment (fitness) -> float

  Calculates the reward or penalty, each of the activated rules recives.
  *fitness* hereby can be used as measure of the performance or whatever
  is suitable in the implementation.
  
  This must be implemented by inheriting classes.

.. method:: CRuleSet.clear () -> None

  Removes all rules from the :class:`CRuleSet`.

.. method:: CRuleSet.distribute_remainder (remainder) -> value

  Distributes the remainder of the weight differences between the
  last weights and current weights.
  
  The method must return a value.
  This must be implemented by inheriting classes.

.. method:: CRuleSet.find (rid) -> :class:`CRule`
        
  Tries to find the :class:`CRule` with the matching id and returns it.
  In case no :class:`CRule` with the passed id exists, None is returned.

.. method:: CRuleSet.remove (rule) -> None

  Removes a :class:`CRule` from the :class:`CRuleSet`.

.. method:: CRuleSet.update_weights (fitness) -> None
        
  Updates the weights of all contained rules.

  Adapted from Pieter Spronck's algorithm as explained in
  Spronck et al: 2005, 'Adaptive Game AI with Dynamic Scripting'.
