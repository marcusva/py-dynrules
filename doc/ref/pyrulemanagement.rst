:class:`RuleManagement` - Rule management
=========================================
.. class:: RuleManagement (id) -> RuleManagement

    Creates a new, empty :class:`RuleManagement` object.

    The :class:`RuleManagement` class takes care of loading and saving
    rules from arbitrary data sources.
    The base is an abstract class, which's
    :meth:`RuleManagement.load_rules()` method must be implemented
    according to the specific needs of the application.

Attributes
----------

.. attribute:: RuleManagement.maxrules

  Gets the maximum amount of rules to manage.
  
Methods
-------

.. method:: RuleManagement.load_rules (maxrules=-1) -> [ Rule, Rule ... ]

  Loads rules from the underlying data source and returns them as list.
  The *maxrules* argument defines the amount of rules to load. If it is
  smaller than 0, all existing rules should be returned.        

  This must be implemented by inheriting classes.
  
.. method:: RuleManagement.save_rules (rules) -> bool
        
  Saves the passed rules to the underlying data source and returns True
  on success or False, if saving the rules was not possible.
        
  This must be implemented by inheriting classes.

.. method:: RuleManagement.save_rules_hint_file (prefix, suffix, learnsystem) -> None
        
  Saves a :class:`LearnSystem`/:class:`RuleSet` combination to a physical file.
  The file can have a specific, unique name in the form: ::
        
    %prefix%_rules_%suffix%.py
