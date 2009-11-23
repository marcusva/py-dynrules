C API reference
===============

The :mod:`dynrules` C API contains fast C based implementations of the dynamic
scripting module API.

Import
------
Include headers::

  dynrules/dynrules.h

.. cfunction:: int import_dynrules (void)

  Imports the :mod:`dynrules` module. This returns 0 on success and -1 on
  failure.

PyRule
------
.. ctype:: PyRule
.. ctype:: Py_Type

The :ctype:`PyRule` object is a simple class type that carries a weight
indicator and arbitrary code data for usage in the dynamic script
generation process.

Members
^^^^^^^
.. cmember:: int PyRule.id

  The unique identifier.

.. cmember:: int PyRule.used

  Indicates whether the rule code was executed. The value is limited to 0 and 1.

.. cmember:: double PyRule.weight

  The weight of the rule.

.. cmember:: PyObject PyRule.code

  The code object to be executed.

Functions
^^^^^^^^^
.. cfunction:: int PyRule_Check (PyObject *obj)

  Returns true, if the argument is a :ctype:`PyRule` or a subclass of
  :ctype:`PyRule`.

.. cfunction:: PyObject* PyRule_New (int id)

  Creates a new :ctype:`PyRule` object with the given id. On failure, this
  returns NULL.

PyRuleSet
---------
.. ctype:: PyRuleSet
.. ctype:: PyRuleSet_Type

The PyRuleSet object is a rule container class that manages rules, their
weights and the weight distribution for the rules.

Members
^^^^^^^
.. cmember:: PyObject PyRuleSet.rules

  A dictionary containing the rules. It uses the :cmember:`PyRule.id` as key and
  the :ctype:`PyRule` as value.

.. cmember:: double PyRuleSet.weight

  The total weight of all contained rules.

.. cmember:: double PyRuleSet.minweight

  The minimum weight a contained :ctype:`PyRule` can retrieve.

.. cmember:: double PyRuleSet.maxweight

  The maximum weight a contained :ctype:`PyRule` can retrieve.

Functions
^^^^^^^^^
.. cfunction:: int PyRuleSet_Check (PyObject *obj)

  Returns true, if the argument is a :ctype:`PyRuleSet` or a subclass of
  :ctype:`PyRuleSet`.

.. cfunction:: PyObject* PyRuleSet_New (double minw, double maxw)

  Creates a new :ctype:`PyRuleSet` object with an initial minimum and maximum
  weight. On failure, this returns NULL.

.. cfunction:: int PyRuleSet_Add (PyObject *ruleset, PyObject *rule)

  Adds a :ctype:`PyRule` to a :ctype:`PyRuleSet`. If there is a rule with the
  same id already in the :ctype:`PyRuleSet`, it will be removed and the passed
  *rule* will be used instead. Additionally the :cmember:`PyRule.weight` member
  will be set to stay within the boundaries of the
  :cmember:`PyRuleSet.minweight` and :cmember:`PyRuleSet.maxweight` limits.
  This returns 1 on success and 0 on failure.
  
.. cfunction:: int PyRuleSet_Remove (PyObject *ruleset, PyObject *rule)

  Removes a :ctype:`PyRule` from a :ctype:`PyRuleSet`. This returns 1 on success
  and 0 on failure.

.. cfunction:: int PyRuleSet_UpdateWeights (PyObject *ruleset, PyObject *fitness)

  Updates the weights of all contained rules. *fitness* will be passed to the
  user-specific :cfunc:`PyRuleSet.calculate_adjustment` method. This returns
  1 on success and 0 on failure.
  
PyLearnSystem
-------------
.. ctype:: PyLearnSystem
.. ctype:: PyLearnSystem_Type

The PyLearnSystem object takes care of creating new scripts  based on a
predefined :ctype:`PyRuleSet`. It does not evaluate the scripts nor modifies
the rules written to them.

Members
^^^^^^^
.. cmember:: PyObject PyLearnSystem.ruleset

  The :ctype:`PyRuleSet` to use.

.. cmember:: int PyLearnSystem.maxtries

  The maximum number of tries to select rules and generate a script.

.. cmember:: int PyLearnSystem.maxscriptsize

  The maximum size in bytes of the scripts to generate.

Functions
^^^^^^^^^
.. cfunction:: int PyLearnSystem_Check (PyObject *obj)

  Returns true, if the argument is a :ctype:`PyLearnSystem` or a subclass of
  :ctype:`PyLearnSystem`.

.. cfunction:: PyObject* PyLearnSystem_New (PyObject *ruleset)

  Creates a new :ctype:`PyLearnSytem` object for the passed :ctype`PyRuleSet`.
  On failure, this returns NULL.

.. cfunction:: int PyLearnSystem_CreateScript (PyObject *learnsystem, PyObject *file, int maxrules)

  Creates a script from the set :cmember:`PyLearnSystem.rulset` using the
  passed script *file*. A maximum of *maxrules* rules will be written.
  *file* can be any file-like Python object or a filename. In case of
  a file object it is assumed to be writeable and won't be closed on
  leaving the function.
