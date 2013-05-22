Python API reference
====================

.. class:: Rule(id : object)

   Creates a new :class:`Rule` object with the given id.
  
   :class:`Rule` is a simple class type that carries a weight indicator
   and arbitrary code data for usage in the dynamic script generation
   process.

   .. attribute:: code

      Gets or sets the code of the :class:`Rule`.
  
   .. attribute:: id

      Gets the id of the :class:`Rule`.
  
   .. attribute:: used

      Indicates whether the :class:`Rule` was used or not.
  
   .. attribute:: weight

      Gets or sets the weight of the :class:`Rule`.

.. class:: RuleSet(minweight : float, maxweight : float)

   Creates a new, empty :class:`RuleSet`.
  
   :class:`RuleSet` is a rule container class that manages rules, their
   weights and the weight distribution for the rules. The *minweight* and
   *maxweight* parameters are the minimum and maximum weight boundaries,
   each rule's weight has to stay in.

   .. attribute:: maxweight

      Gets or sets the maximum weight to use for rules.
  
   .. attribute:: minweight

      Gets or sets the minimum weight to use for rules.

   .. attribute:: rules

      Gets the list of currently managed :class:`Rule` objects.

   .. attribute:: weight

      Gets the total weight of all managed :class:`Rule` objects.

   .. method:: add(rule : Rule)

      Adds a :class:`Rule` to the :class:`RuleSet`.

   .. method:: calculate_adjustment(fitness : float) -> float

      Calculates the reward or penalty, each of the activated rules recives.
      *fitness* hereby can be used as measure of the performance or whatever
      is suitable in the implementation.
  
      This must be implemented by inheriting classes.

   .. method:: clear()

      Removes all rules from the :class:`RuleSet`.

   .. method:: distribute_remainder(remainder : float) -> float

      Distributes the remainder of the weight differences between the
      last weights and current weights.
  
      The method must return a value.
      This must be implemented by inheriting classes.

   .. method:: find(rid : float) -> Rule
        
      Tries to find the :class:`Rule` with the matching id and returns it.
      In case no :class:`Rule` with the passed id exists, None is returned.

   .. method:: RuleSet.remove(rule : Rule)

      Removes a :class:`Rule` from the :class:`RuleSet`.

   .. method:: RuleSet.update_weights(fitness : float)
        
      Updates the weights of all contained rules.

      Adapted from Pieter Spronck's algorithm as explained in
      Spronck et al: 2005, 'Adaptive Game AI with Dynamic Scripting'.

.. class:: RuleManager(id : object)

   The :class:`RuleManager` class takes care of loading and saving
   rules from arbitrary data sources.
   The base is an abstract class, which's :meth:`load_rules` method must be
   implemented according to the specific needs of the application.

   .. attribute:: maxrules

      Gets the maximum amount of rules to manage.
  
   .. method:: load_rules([maxrules=-1]) -> [Rule, Rule ...]

      Loads rules from the underlying data source and returns them as list.
      The *maxrules* argument defines the amount of rules to load. If it is
      smaller than 0, all existing rules should be returned.        

      This must be implemented by inheriting classes.
  
   .. method:: save_rules(rules : iterable)
        
      Saves the passed rules to the underlying data source.
        
      This must be implemented by inheriting classes.

   .. method:: save_rules_hint_file(filename : string, learnsystem : LearnSystem)
        
      Saves a :class:`LearnSystem`/:class:`RuleSet` combination to a physical
      file.

.. class:: MMapRuleManager(maxrules : int)
    
   A simple memory-mapped :class:`RuleManager` implementation that
   does not load its rules from an external data source.

   It is an extremely useful class for testing rules and basic algorithms, but
   due to the in-memory management of all rules, it should not be used in a
   productive environment, especially if large rule sets have to be managed.

   By default, the :class:`MMapRuleManager` class will reserve enough memory
   for the rules to manage, when it is constructed. It will **not** fill the
   rules with useful values though. It is up to caller to use
   :meth:`load_rules()` afterwards and fill the returned :class:`Rule`
   instances with the necessary data.

   .. attribute:: maxrules

      Gets the maximum amount of rules to manage.
  
   .. method:: load_rules([maxrules=-1]) -> [Rule, Rule ...]

      Returns the internally managed rules or a certain subset.
  
   .. method:: save_rules(rules : iterable)
        
      This does nothing and will always return True.

   .. method:: save_rules_hint_file(filename : string, learnsystem : LearnSystem)
        
      Saves a :class:`LearnSystem`/:class:`RuleSet` combination to a physical
      file.

.. class:: LearnSystem(ruleset : RuleSet)

   Creates a new :class:`LearnSystem` using a specific :class:`RuleSet`.

   The :class:`LearnSystem` class takes care of creating new scripts
   based on a predefined :class:`RuleSet`. It does not evaluate the
   scripts nor modifies the rules written to them.

   The procedure of creating scripts is done using three phases:
 
   * header creation
   * rule code creation
   * footer creation

   The header and footer are freely choosable. You can simple override
   or reassign the :meth:`create_header()` and :meth:`create_footer()` methods
   to let them return your required code.

   .. attribute:: maxscriptsize

      Gets or sets the maximum script size (in bytes) for inserting rules.
  
   .. attribute:: maxtries

      Gets or sets the maximum amount of tries to insert a script rule.
  
   .. attribute:: ruleset

      Gets or sets the :class:`RuleSet` to use.

   .. method:: create_footer() -> str

      Creates the footer for the script file.
  
      The default implementation does nothing.

   .. method:: create_header() -> str
    
      Creates the header for the script file.
  
      The default implementation does nothing.  

   .. method:: create_rules(maxrules : int) -> str
  
      Creates a rule list from the currently active :class:`RuleSet`.
      Gets *maxrules* rules from the set :class:`RuleSet` and passes
      their code back as string for the script file.

      Adapted from Pieter Spronck's algorithm as explained in
      Spronck et al: 2005, 'Adaptive Game AI with Dynamic Scripting'.

   .. method:: create_script(scriptfile : object, maxrules : int)
  
      Creates a script from the available :class:`RuleSet` using the
      passed script file. A maximum of *maxrules* rules will be written.
      *scriptfile* can be either a file object or filename. In case of
      a file object it is assumed to be writeable and won't be closed on
      leaving the function (but flushed).
