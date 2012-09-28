/*
 * dynrules - Python dynamic rules engine
 *
 * Authors: Marcus von Appen
 *
 * This file is distributed under the Public Domain.
 */

#ifndef _DYNRULES_H_
#define _DYNRULES_H_

#include <Python.h>
#include "compat.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct
{
    PyObject_HEAD

    PyObject *id;
    int       used : 1;
    double    weight;
    PyObject *code;
    PyObject *dict;
} PyRule;

#define DYNRULES_RULE_FIRSTSLOT 0
#define DYNRULES_RULE_NUMSLOTS 2
#ifndef DYNRULES_RULE_INTERNAL
#define  PyRule_Type                                                   \
    (*(PyTypeObject*)PyDynRules_C_API[DYNRULES_RULE_FIRSTSLOT+0])
#define PyRule_Check(x)                                                \
    (PyObject_TypeCheck(x,                                             \
        (PyTypeObject*)PyDynRules_C_API[DYNRULES_RULE_FIRSTSLOT+0]))
#define PyRule_New                                                     \
    (*(PyObject*(*)(PyObject*)) PyDynRules_C_API[DYNRULES_RULE_FIRSTSLOT+1])
#endif /* DYNRULES_RULE_INTERNAL */


typedef struct
{
    PyObject_HEAD

    PyObject *rules;
    double    weight;
    double    minweight;
    double    maxweight;
    PyObject *dict;
} PyRuleSet;

#define DYNRULES_RULESET_FIRSTSLOT \
    (DYNRULES_RULE_FIRSTSLOT + DYNRULES_RULE_NUMSLOTS)
#define DYNRULES_RULESET_NUMSLOTS 5
#ifndef DYNRULES_RULESET_INTERNAL
#define  PyRuleSet_Type                                                 \
    (*(PyTypeObject*)PyDynRules_C_API[DYNRULES_RULESET_FIRSTSLOT+0])
#define PyRuleSet_Check(x)                                              \
    (PyObject_TypeCheck(x,                                              \
        (PyTypeObject*)PyDynRules_C_API[DYNRULES_RULESET_FIRSTSLOT+0]))
#define PyRuleSet_New                                                   \
    (*(PyObject*(*)(double,double)) PyDynRules_C_API[DYNRULES_RULESET_FIRSTSLOT+1])
#define PyRuleSet_Add                                                   \
    (*(int(*)(PyObject*,PyObject*)) PyDynRules_C_API[DYNRULES_RULESET_FIRSTSLOT+2])
#define PyRuleSet_Remove                                                \
    (*(int(*)(PyObject*,PyObject*)) PyDynRules_C_API[DYNRULES_RULESET_FIRSTSLOT+3])
#define PyRuleSet_UpdateWeights                                         \
    (*(int(*)(PyObject*,PyObject*)) PyDynRules_C_API[DYNRULES_RULESET_FIRSTSLOT+4])
#endif /* DYNRULES_RULESET_INTERNAL */

typedef struct
{
    PyObject_HEAD
    
    PyObject *ruleset;
    int       maxtries;
    int       maxscriptsize;
    PyObject *dict;
} PyLearnSystem;
#define DYNRULES_LEARNSYSTEM_FIRSTSLOT \
    (DYNRULES_RULESET_FIRSTSLOT + DYNRULES_RULESET_NUMSLOTS)
#define DYNRULES_LEARNSYSTEM_NUMSLOTS 3
#ifndef DYNRULES_LEARNSYSTEM_INTERNAL
#define  PyLearnSystem_Type                                             \
    (*(PyTypeObject*)PyDynRules_C_API[DYNRULES_LEARNSYSTEM_FIRSTSLOT+0])
#define PyLearnSystem_Check(x)                                          \
    (PyObject_TypeCheck(x,                                              \
        (PyTypeObject*)PyDynRules_C_API[DYNRULES_LEARNSYSTEM_FIRSTSLOT+0]))
#define PyLearnSystem_New                                               \
    (*(PyObject*(*)(PyObject*)) PyDynRules_C_API[DYNRULES_LEARNSYSTEM_FIRSTSLOT+1])
#define PyLearnSystem_CreateScript                                      \
    (*(int(*)(PyObject*,PyObject*,int)) PyDynRules_C_API[DYNRULES_LEARNSYSTEM_FIRSTSLOT+2])
#endif /* DYNRULES_LEARNSYSTEM_INTERNAL */

/**
 * C API export.
 */
static void **PyDynRules_C_API;

#define DYNRULES_SLOTS \
    (DYNRULES_LEARNSYSTEM_FIRSTSLOT + DYNRULES_LEARNSYSTEM_NUMSLOTS)
#define DYNRULES_ENTRY "_DYNRULES_CAPI"
#define DYNRULES_CMOD_ENTRY "dynrules._dynrules._DYNRULES_CAPI"

static int
import_dynrules (void)
{
#if PY_VERSION_HEX >= 0x03010000
    PyObject *_module = PyImport_ImportModule ("dynrules._dynrules");
    if (_module == NULL)
        return -1;
    PyDynRules_C_API = (void**) PyCapsule_Import (DYNRULES_CMOD_ENTRY, 0);
    return (PyDynRules_C_API != NULL) ? 0 : -1;
#else
    PyObject *_module = PyImport_ImportModule ("dynrules._dynrules");
    if (_module != NULL)
    {
        PyObject *_capi = PyObject_GetAttrString(_module, DYNRULES_ENTRY);
        if (!PyCObject_Check (_capi))
        {
            Py_DECREF (_module);
            return -1;
        }
        PyDynRules_C_API = (void**) PyCObject_AsVoidPtr (_capi);
        Py_DECREF (_capi);
        return 0;
    }
    return -1;
#endif
}


#ifdef __cplusplus
}
#endif

#endif /* _DYNRULES_H_ */
