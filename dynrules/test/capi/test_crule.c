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
test_crule (void)
{
    PyObject *rule;
    PyObject *lid = PyLong_FromLong (3);
    
    rule = PyRule_New (lid);
    if (!PyRule_Check (rule))
        ERROR ("Rule mismatch in PyRule_Check");
    if (((PyRule*) rule)->id != lid ||
        ((PyRule*) rule)->used != 0 ||
        ((PyRule*) rule)->weight != 0.f ||
        ((PyRule*) rule)->code != NULL)
        ERROR ("Rule mismatch in PyRule_New");
    
    Py_DECREF (rule);
}

int
main (int argc, char *argv[])
{
    Py_Initialize ();

    if (import_dynrules () == -1)
        ERROR ("Could not import dynrules");
    test_crule ();
    Py_Finalize ();
    return 0;
}
