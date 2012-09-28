#include <stdio.h>
#include <stdlib.h>
#include <dynrules/dynrules.h>

#define ERROR(x)                            \
    {                                       \
        fprintf (stderr, "*** %s\n", x);    \
        PyErr_Print ();                     \
        Py_Finalize ();                     \
        exit (1);                           \
    }

static void
test_cruleset (void)
{
    PyObject *ruleset;
    
    ruleset = PyRuleSet_New (0.f, 10.f);
    if (!PyRuleSet_Check (ruleset))
        ERROR ("RuleSet mismatch in PyRuleSet_Check");
    if (((PyRuleSet*) ruleset)->minweight != 0.f ||
        ((PyRuleSet*) ruleset)->maxweight != 10.f ||
        ((PyRuleSet*) ruleset)->weight != 0.f)
        ERROR ("RuleSet mismatch in PyRuleSet_New");
    Py_DECREF (ruleset);
    
    ruleset = PyRuleSet_New (-3.214, 0.333);
    if (!PyRuleSet_Check (ruleset))
        ERROR ("RuleSet mismatch in PyRuleSet_Check");
    if (((PyRuleSet*) ruleset)->minweight != -3.214 ||
        ((PyRuleSet*) ruleset)->maxweight != 0.333 ||
        ((PyRuleSet*) ruleset)->weight != 0.f)
        ERROR ("RuleSet mismatch in PyRuleSet_New");
    Py_DECREF (ruleset);
}

int
main (int argc, char *argv[])
{
    Py_Initialize ();

    if (import_dynrules () == -1)
        ERROR ("Could not import dynrules");
    test_cruleset ();
    Py_Finalize ();
    return 0;
}
