:class:`RuleSet` - Rule container
==================================
.. class:: RuleSet (minweight, maxweight) -> RuleSet

  Creates a new, empty :class:`RuleSet`.
  
  :class:`RuleSet` is a rule container class that manages rules, their
  weights and the weight distribution for the rules. The *minweight* and
  *maxweight* parameters are the minimum and maximum weight boundaries,
  each rule's weight has to stay in.

Attributes
----------

.. attribute:: RuleSet.maxweight

  Gets or sets the maximum weight to use for rules.
  
.. attribute:: RuleSet.minweight

  Gets or sets the minimum weight to use for rules.

.. attribute:: RuleSet.rules

  Gets the list of currently managed :class:`Rule` objects.

.. attribute:: RuleSet.weight

  Gets the total weight of all managed :class:`Rule` objects.

Methods
-------

.. method:: RuleSet.add (rule) -> None

  Adds a :class:`Rule` to the :class:`RuleSet`.

.. method:: RuleSet.calculate_adjustment (fitness) -> float

  Calculates the reward or penalty, each of the activated rules recives.
  *fitness* hereby can be used as measure of the performance or whatever
  is suitable in the implementation.
  
  This must be implemented by inheriting classes.

.. method:: RuleSet.clear () -> None

  Removes all rules from the :class:`RuleSet`.

.. method:: RuleSet.distribute_remainder (remainder) -> value

  Distributes the remainder of the weight differences between the
  last weights and current weights.
  
  The method must return a value.
  This must be implemented by inheriting classes.

.. method:: RuleSet.find (rid) -> :class:`Rule`
        
  Tries to find the :class:`Rule` with the matching id and returns it.
  In case no :class:`Rule` with the passed id exists, None is returned.

.. method:: RuleSet.remove (rule) -> None

  Removes a :class:`Rule` from the :class:`RuleSet`.

.. method:: RuleSet.update_weights (fitness) -> None
        
  Updates the weights of all contained rules.

  Adapted from Pieter Spronck's algorithm as explained in
  Spronck et al: 2005, 'Adaptive Game AI with Dynamic Scripting'.
