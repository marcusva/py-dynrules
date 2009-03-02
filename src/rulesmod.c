/*
 * dynrules - Python dynamic rules engine
 *
 * Authors: Marcus von Appen
 *
 * This file is distributed under the Public Domain.
 */

#define DYNRULES_RULESMOD_INTERNAL

#include "rulesmod.h"
#include "dynrules.h"

int
get_double_from_obj (PyObject *obj, double *value)
{
    PyObject* val;
    double result;

    if (PyNumber_Check (obj))
    {
        val = PyNumber_Float (obj);
        if (!val)
            return 0;

        result = PyFloat_AsDouble (obj);
        Py_DECREF (val);

        if (result == -1 && PyErr_Occurred ())
            return 0;

        *value = result;
        return 1;
    }
    return 0;
}

int
get_int_from_obj (PyObject *obj, int *value)
{
    PyObject* val;
    long result;

    if (PyNumber_Check (obj))
    {
        val = PyNumber_Long (obj);
        if (!val)
            return 0;
        result = PyLong_AsLong (obj);
        Py_DECREF (val);

        if (result == -1 && PyErr_Occurred ())
            return 0;

        *value = (int) result;
        return 1;
    }
    return 0;
}

#if PY_VERSION_HEX < 0x03000000
struct PycStringIO_CAPI*
get_stringio_api (void)
{
    return PycStringIO;
}
#endif

#if PY_VERSION_HEX >= 0x03000000
PyMODINIT_FUNC PyInit__dynrules (void)
#else
PyMODINIT_FUNC init_dynrules  (void)
#endif
{

#if PY_VERSION_HEX >= 0x03000000
    static struct PyModuleDef _dynrulesmodule = {
        PyModuleDef_HEAD_INIT, "_dynrules",
        "The C implementation for the dynrules package", -1,
        NULL, NULL, NULL, NULL, NULL
    };
#endif
    static void* c_api[DYNRULES_SLOTS];
    PyObject *mod, *c_api_obj;
    
    /* Complete types */
    PyRule_Type.tp_new = PyType_GenericNew;
    if (PyType_Ready (&PyRule_Type) < 0)
        MOD_RETURN(NULL);
    PyRuleSet_Type.tp_new = PyType_GenericNew;
    if (PyType_Ready (&PyRuleSet_Type) < 0)
        MOD_RETURN(NULL);
    PyLearnSystem_Type.tp_new = PyType_GenericNew;
    if (PyType_Ready (&PyLearnSystem_Type) < 0)
        MOD_RETURN(NULL);

    Py_INCREF (&PyRule_Type);
    Py_INCREF (&PyRuleSet_Type);
    Py_INCREF (&PyLearnSystem_Type);

#if PY_VERSION_HEX < 0x03000000
    mod = Py_InitModule3 ("_dynrules", NULL,
        "The C implementation for the dynrules package");
#else
    mod = PyModule_Create (&_dynrulesmodule);
#endif
    if (!mod)
        MOD_RETURN(NULL);

    PyModule_AddObject (mod, "Rule", (PyObject *) &PyRule_Type);
    PyModule_AddObject (mod, "RuleSet", (PyObject *) &PyRuleSet_Type);
    PyModule_AddObject (mod, "LearnSystem", (PyObject *) &PyLearnSystem_Type);

    rule_export_capi (c_api);
    ruleset_export_capi (c_api);
    learnsystem_export_capi (c_api);
    
    c_api_obj = PyCObject_FromVoidPtr ((void *) c_api, NULL);
    if (c_api_obj)
        PyModule_AddObject (mod, DYNRULES_ENTRY, c_api_obj);

    /* cStringIO import */
#if PY_VERSION_HEX < 0x03000000
    PycString_IMPORT;
#endif
    MOD_RETURN(mod);
}
