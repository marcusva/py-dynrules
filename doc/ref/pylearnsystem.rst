:class:`LearnSystem` - Rule script generation
==============================================
.. class:: LearnSystem (ruleset) -> LearnSystem

  Creates a new :class:`LearnSystem` using a specific :class:`RuleSet`.

  The :class:`LearnSystem` class takes care of creating new scripts
  based on a predefined :class:`RuleSet`. It does not evaluate the
  scripts nor modifies the rules written to them.

  The procedure of creating scripts is done using three phases:

  * header creation
  * rule code creation
  * footer creation

  The header and footer are freely choosable. You can simple override
  or reassign the :meth:`LearnSystem.create_header()` and
  :meth:`LearnSystem.create_footer()` methods to let them return your
  required code.

Attributes
----------

.. attribute:: LearnSystem.maxscriptsize

  Gets or sets the maximum script size (in bytes) for inserting rules.
  
.. attribute:: LearnSystem.maxtries

  Gets or sets the maximum amount of tries to insert a script rule.
  
.. attribute:: LearnSystem.ruleset

  Gets or sets the :class:`RuleSet` to use.

Methods
-------

.. method:: LearnSystem.create_footer () -> str

  Creates the footer for the script file.
  
  The default implementation does nothing.

.. method:: LearnSystem.create_header () -> str
    
  Creates the header for the script file.
  
  The default implementation does nothing.  

.. method:: LearnSystem.create_rules (maxrules) -> str
  
  Creates a rule list from the currently active :class:`RuleSet`.
  Gets *maxrules* rules from the set :class:`RuleSet` and passes
  their code back as string for the script file.

  Adapted from Pieter Spronck's algorithm as explained in
  Spronck et al: 2005, 'Adaptive Game AI with Dynamic Scripting'.

.. method:: LearnSystem.create_script (scriptfile, maxrules) -> None
  
  Creates a script from the available :class:`RuleSet` using the
  passed script file. A maximum of *maxrules* rules will be written.
  *scriptfile* can be either a file object or filename. In case of
  a file object it is assumed to be writeable and won't be closed on
  leaving the function (but flushed).
 