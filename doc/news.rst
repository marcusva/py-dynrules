Release News
============
This describes the latest changes between the dynrules releases.

0.1.0
-----
Released on 2013-05-22

Python framework: 
  * Removed Python C module.
  * Renamed RuleManagement class to RuleManager.
  * Renamed MMapRuleManagement class to MMapRuleManager.

C++ framework:
  * Renamed RuleManagement class to RuleManager.
  * Renamed MMapRuleManagement class to MMapRuleManager.

0.0.15
------
Released on 2012-09-30

Python framework:
  * Added PyPy support.
  * More compatibility fixes for Python 3.x.
  * Fixed a refcount issue in CRuleSet.update_weights(), which might
    lead to crashes.
  * Moved unit tests into dynrules package.

0.0.14
------
Released on 2011-05-14

Python framework:
  * Compatibility fixes for Python 3.2.

0.0.13
------
Released on 2010-05-19

Python framework:
  * Fixed constructor for RuleManagement class.

0.0.12
------
Released on 2010-03-26

Python framework:
  * Fixed file recognition issue for Python 3.x in CLearnSystem.

0.0.11
------
Released on 2009-12-25

* Fixed documentation distribution.

0.0.10
------
Released on 2009-12-24

Python framework:
  * Changed CRule.id to be an arbitrary object.
  * Fixed minweight/maxweight range checks for CRuleSet.
  * Fixed minweight/maxweight checks within the C API interfaces.

0.0.9
-----
Released on 2009-12-22

C++ framework:
  * Changed RuleManagement.save_rules_hint_file() to take a filename instead
    of prefix and suffix.
  * Better Makefile support
  * Fixed several pointer/const/reference issues.
  * Changed char* exceptions to be invalid_argument types.
  * Added HTML API reference.
  * Added Win32 VC++ (VS.NET 2008) support.

Python framework:
  * Changed RuleManagement.save_rules_hint_file() to take a filename instead
    of prefix and suffix.
  * Added C API documentation.
  * Added C API tests.
  * import cleanups.

0.0.8
-----
Released on 2009-07-03

Python framework:
  * Fixed an import bug in LearnSystem.

0.0.7
-----
Released on 2009-03-09

C++ framework:
  * Added missing RuleManagement::getMaxRules() method.
  * Fixed documentation comments.
  * RuleSet::updateWeights() method receives only a fitness argument now.

Python framework:
  * Many fixes for correct Python 3.x support.
  * CLearnSystem.create_script() now can handle file names properly.
  * Added API reference to documentations.

0.0.6
-----
Released on 2008-12-16

Python framework:
  * Added Python 3.x support.
    Note that CLearnSystem.create_script() does not accept file names with
    Python 3.0, only file objects.

0.0.5
-----
Released on 2008-11-21

C++ framework:
  * Fixed a bug in RuleSet::updateWeights() which caused wrong weight
    results.
  * Added missing MMapRuleManagement.h include to dynrules.h

Python framework:
  * Fixed name ambiguity for the Python and C implementation.
    The visible C types in the dynrules packages were renamed to
    CRule, CRuleSet and CLearnSystem, the C module types still have their
    original name.
  * Fixed C API slots.
  * Added RuleSet.find() and CRuleSet.find() methods.
  * New RuleManagement class.
  * New MMapRuleManagement class for in-memory rule management.

0.0.4
-----
Released on 2008-11-20

C++ framework:
  * New RuleSet.find() method.
  * New abstract RuleManagement class for managing rules.
  * New MMapRuleManagement class for in-memory rule management.
  * Changed API to pass object pointers around instead of objects.

0.0.3
-----
Released on 2008-11-09

C++ framework:
  * Added documentation.
  * Fixed a minor range issue in the RuleSet constructor that allowed
    minweight to be smaller than maxweight.
  * Fixed an int vs. double bug in the LearnSystem constructor.

0.0.2
-----
Released on 2008-11-08

* New pure C++ framework under cplusplus

0.0.1
-----
Released on 2008-10-06

* Initial release.
