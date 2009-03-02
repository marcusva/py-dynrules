:class:`MMapRuleManagement` - Memory-mapped rule management
===========================================================
.. class:: MMapRuleManagement (maxrules) -> MMapRuleManagement
    
    Creates a new :class:`MMapRuleManagement` instance.
    
    A simple memory-mapped :class:`RuleManagement` implementation that
    does not load its rules from an external data source.

    It is an extremely useful class for testing rules and basic algorithms, but
    due to the in-memory management of all rules, it should not be used in a
    productive environment, especially if large rule sets have to be managed.

    By default, the :class:`MMapRuleManagement` class will reserve enough memory
    for the rules to manage, when it is constructed. It will **not** fill the
    rules with useful values though. It is up to caller to use
    :meth:`MMapRuleManagement.load_rules()` afterwards and fill the returned
    :class:`Rule` instances with the necessary data.

Attributes
----------

.. attribute:: MMapRuleManagement.maxrules

  Gets the maximum amount of rules to manage.
  
Methods
-------

.. method:: MMapRuleManagement.load_rules (maxrules=-1) -> [ Rule, Rule ... ]

  Returns the internally managed rules or a certain subset.
  
.. method:: MMapRuleManagement.save_rules (rules) -> bool
        
  This does nothing and will always return True.

.. method:: MMapRuleManagement.save_rules_hint_file (prefix, suffix, learnsystem) -> None
        
  Saves a :class:`LearnSystem`/:class:`RuleSet` combination to a physical file.
  The file can have a specific, unique name in the form: ::
        
    %prefix%_rules_%suffix%.py
