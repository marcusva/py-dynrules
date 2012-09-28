:class:`CLearnSystem` - Rule script generation
==============================================
.. class:: CLearnSystem (ruleset) -> CLearnSystem

  Creates a new :class:`CLearnSystem` using a specific :class:`CRuleSet`.

  The :class:`CLearnSystem` class takes care of creating new scripts
  based on a predefined :class:`CRuleSet`. It does not evaluate the
  scripts nor modifies the rules written to them.

  The procedure of creating scripts is done using three phases:

  * header creation
  * rule code creation
  * footer creation

  The header and footer are freely choosable. You can simple override
  or reassign the :meth:`CLearnSystem.create_header()` and
  :meth:`CLearnSystem.create_footer()` methods to let them return your
  required code.

Attributes
----------

.. attribute:: CLearnSystem.maxscriptsize

  Gets or sets the maximum script size (in bytes) for inserting rules.
  
.. attribute:: CLearnSystem.maxtries

  Gets or sets the maximum amount of tries to insert a script rule.
  
.. attribute:: CLearnSystem.ruleset

  Gets or sets the :class:`CRuleSet` to use.

Methods
-------

.. method:: CLearnSystem.create_footer () -> str

  Creates the footer for the script file.
  
  The default implementation does nothing.

.. method:: CLearnSystem.create_header () -> str
    
  Creates the header for the script file.
  
  The default implementation does nothing.  

.. method:: CLearnSystem.create_rules (maxrules) -> str
  
  Creates a rule list from the currently active :class:`CRuleSet`.
  Gets *maxrules* rules from the set :class:`CRuleSet` and passes
  their code back as string for the script file.

  Adapted from Pieter Spronck's algorithm as explained in
  Spronck et al: 2005, 'Adaptive Game AI with Dynamic Scripting'.

.. method:: CLearnSystem.create_script (scriptfile, maxrules) -> None
  
  Creates a script from the available :class:`CRuleSet` using the
  passed script file. A maximum of *maxrules* rules will be written.
  *scriptfile* can be either a file object or filename. In case of
  a file object it is assumed to be writeable and won't be closed on
  leaving the function (but flushed).
 