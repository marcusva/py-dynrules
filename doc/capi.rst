.. default-domain:: c

C API reference
===============

The :py:mod:`dynrules` C API contains fast C based implementations of
the dynamic scripting module API.


Import
------
Include headers::

  dynrules/dynrules.h

.. function:: int import_dynrules (void)

  Imports the :py:mod:`dynrules` module. This returns 0 on success and -1 on
  failure.

PyRule
------
.. type:: PyRule
.. type:: Py_Type

The :type:`PyRule` object is a simple class type that carries a weight
indicator and arbitrary code data for usage in the dynamic script
generation process.

Members
^^^^^^^
.. member:: PyObject PyRule.id

  The unique identifier.

.. member:: int PyRule.used

  Indicates whether the rule code was executed. The value is limited to 0 and 1.

.. member:: double PyRule.weight

  The weight of the rule.

.. member:: PyObject PyRule.code

  The code object to be executed.

Functions
^^^^^^^^^
.. function:: int PyRule_Check (PyObject *obj)

  Returns true, if the argument is a :type:`PyRule` or a subclass of
  :type:`PyRule`.

.. function:: PyObject* PyRule_New (PyObject *id)

  Creates a new :type:`PyRule` object with the given id. On failure, this
  returns NULL.

PyRuleSet
---------
.. type:: PyRuleSet
.. type:: PyRuleSet_Type

The PyRuleSet object is a rule container class that manages rules, their
weights and the weight distribution for the rules.

Members
^^^^^^^
.. member:: PyObject PyRuleSet.rules

  A dictionary containing the rules. It uses the :member:`PyRule.id` as key and
  the :type:`PyRule` as value.

.. member:: double PyRuleSet.weight

  The total weight of all contained rules.

.. member:: double PyRuleSet.minweight

  The minimum weight a contained :type:`PyRule` can retrieve.

.. member:: double PyRuleSet.maxweight

  The maximum weight a contained :type:`PyRule` can retrieve.

Functions
^^^^^^^^^
.. function:: int PyRuleSet_Check (PyObject *obj)

  Returns true, if the argument is a :type:`PyRuleSet` or a subclass of
  :type:`PyRuleSet`.

.. function:: PyObject* PyRuleSet_New (double minw, double maxw)

  Creates a new :type:`PyRuleSet` object with an initial minimum and maximum
  weight. On failure, this returns NULL.

.. function:: int PyRuleSet_Add (PyObject *ruleset, PyObject *rule)

  Adds a :type:`PyRule` to a :type:`PyRuleSet`. If there is a rule with the
  same id already in the :type:`PyRuleSet`, it will be removed and the passed
  *rule* will be used instead. Additionally the :member:`PyRule.weight` member
  will be set to stay within the boundaries of the
  :member:`PyRuleSet.minweight` and :member:`PyRuleSet.maxweight` limits.
  This returns 1 on success and 0 on failure.

.. function:: int PyRuleSet_Remove (PyObject *ruleset, PyObject *rule)

  Removes a :type:`PyRule` from a :type:`PyRuleSet`. This returns 1 on success
  and 0 on failure.

.. function:: int PyRuleSet_UpdateWeights (PyObject *ruleset, PyObject *fitness)

  Updates the weights of all contained rules. *fitness* will be passed
  to the user-specific :func:`PyRuleSet.calculate_adjustment`
  method. This returns 1 on success and 0 on failure.

PyLearnSystem
-------------
.. type:: PyLearnSystem
.. type:: PyLearnSystem_Type

The PyLearnSystem object takes care of creating new scripts based on a
predefined :type:`PyRuleSet`. It does not evaluate the scripts nor
modifies the rules written to them.

Members
^^^^^^^
.. member:: PyObject PyLearnSystem.ruleset

   The :type:`PyRuleSet` to use.

.. member:: int PyLearnSystem.maxtries

   The maximum number of tries to select rules and generate a script.

.. member:: int PyLearnSystem.maxscriptsize

   The maximum size in bytes of the scripts to generate.

Functions
^^^^^^^^^
.. function:: int PyLearnSystem_Check (PyObject *obj)

   Returns true, if the argument is a :type:`PyLearnSystem` or a subclass of
   :type:`PyLearnSystem`.

.. function:: PyObject* PyLearnSystem_New (PyObject *ruleset)

   Creates a new :type:`PyLearnSytem` object for the passed :type:`PyRuleSet`.
   On failure, this returns NULL.

.. function:: int PyLearnSystem_CreateScript (PyObject *learnsystem, \
   PyObject *file, int maxrules)

   Creates a script from the set :member:`PyLearnSystem.rulset` using the
   passed script *file*. A maximum of *maxrules* rules will be written.
   *file* can be any file-like Python object or a filename. In case of
   a file object it is assumed to be writeable and won't be closed on
   leaving the function.
