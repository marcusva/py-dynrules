/*
 * dynrules - Python dynamic rules engine
 *
 * Authors: Marcus von Appen
 *
 * This file is distributed under the Public Domain.
 */

#ifndef _RULESMOD_H_
#define _RULESMOD_H_

#include <Python.h>
#include <structmember.h>

#if PY_VERSION_HEX < 0x03000000
#include <cStringIO.h>
#endif

#define DYNRULES_RULE_INTERNAL
#define DYNRULES_RULESET_INTERNAL
#define DYNRULES_LEARNSYSTEM_INTERNAL

#if PY_VERSION_HEX < 0x03000000
struct PycStringIO_CAPI* get_stringio_api (void);
#endif

int get_double_from_obj (PyObject *obj, double *value);
int get_int_from_obj (PyObject *obj, int *value);

extern PyTypeObject PyRule_Type;
#define PyRule_Check(x) (PyObject_TypeCheck(x, &PyRule_Type))
void rule_export_capi (void **capi);

extern PyTypeObject PyRuleSet_Type;
#define PyRuleSet_Check(x) (PyObject_TypeCheck(x, &PyRuleSet_Type))
void ruleset_export_capi (void **capi);

extern PyTypeObject PyLearnSystem_Type;
#define PyLearnSystem_Check(x) (PyObject_TypeCheck(x, &PyLearnSystem_Type))
void learnsystem_export_capi (void **capi);

#endif /* _RULESMOD_H_ */
