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
    PyErr_SetString (PyExc_TypeError, "value must be a number");
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
    PyErr_SetString (PyExc_TypeError, "value must be a number");
    return 0;
}

#if PY_VERSION_HEX < 0x03000000 && !defined(PYPY_VERSION)
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

#if PY_VERSION_HEX < 0x03000000
    mod = Py_InitModule3 ("_dynrules", NULL,
        "The C implementation for the dynrules package");
#else
    mod = PyModule_Create (&_dynrulesmodule);
#endif
    if (!mod)
        MOD_RETURN(NULL);
    ADD_OBJ_OR_FAIL (mod, "Rule", PyRule_Type, failed);
    ADD_OBJ_OR_FAIL (mod, "RuleSet", PyRuleSet_Type, failed);
    ADD_OBJ_OR_FAIL (mod, "LearnSystem", PyLearnSystem_Type, failed);

    rule_export_capi (c_api);
    ruleset_export_capi (c_api);
    learnsystem_export_capi (c_api);

#if PY_VERSION_HEX >= 0x03010000
    c_api_obj = PyCapsule_New((void *)c_api, DYNRULES_CMOD_ENTRY, NULL);
#else
    c_api_obj = PyCObject_FromVoidPtr ((void *) c_api, NULL);
#endif
    if (c_api_obj)
    {
        if (PyModule_AddObject (mod, DYNRULES_ENTRY, c_api_obj) == -1)
        {
            Py_DECREF (c_api_obj);
            goto failed;
        }
    }
    else
    {
        goto failed;
    }

    /* cStringIO import */
#if PY_VERSION_HEX < 0x03000000 && !defined(PYPY_VERSION)
    PycString_IMPORT;
#endif
    MOD_RETURN(mod);

failed:
    Py_DECREF (mod);
    MOD_RETURN (NULL);
}
