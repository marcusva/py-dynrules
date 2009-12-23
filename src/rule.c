/*
 * dynrules - Python dynamic rules engine
 *
 * Authors: Marcus von Appen
 *
 * This file is distributed under the Public Domain.
 */

#define DYNRULES_RULE_INTERNAL

#include "rulesmod.h"
#include "dynrules.h"

static int _rule_init (PyObject *rule, PyObject *args, PyObject *kwds);
static void _rule_dealloc (PyRule *rule);

/* Getters/Setters */
static PyObject* _rule_getdict (PyRule *rule, void *closure);
static PyObject* _rule_getid (PyRule *rule, void *closure);
static PyObject* _rule_getweight (PyRule *rule, void *closure);
static int _rule_setweight (PyRule *rule, PyObject *value, void *closure);
static PyObject* _rule_getcode (PyRule *rule, void *closure);
static int _rule_setcode (PyRule *rule, PyObject *value, void *closure);
static PyObject* _rule_getused (PyRule *rule, void *closure);
static int _rule_setused (PyRule *rule, PyObject *value, void *closure);

/* C API */
static PyObject* PyRule_New (int id);

/**
 * Methods bound to the PyRule type.
 */
static PyMethodDef _rule_methods[] =
{
    { NULL, NULL, 0, NULL }
};

/**
 * Getters and setters for the PyRule type.
 */
static PyGetSetDef _rule_getsets[] =
{
    { "__dict__", (getter) _rule_getdict, NULL, NULL, NULL },
    { "id", (getter) _rule_getid, NULL, "", NULL },
    { "weight", (getter) _rule_getweight, (setter) _rule_setweight, "", NULL },
    { "code", (getter) _rule_getcode, (setter) _rule_setcode, "", NULL },
    { "used", (getter) _rule_getused, (setter) _rule_setused, "", NULL },
    { NULL, NULL, NULL, NULL, NULL }
};

PyTypeObject PyRule_Type =
{
    TYPE_HEAD(NULL,0)
    "_dynrules.Rule",           /* tp_name */
    sizeof (PyRule),            /* tp_basicsize */
    0,                          /* tp_itemsize */
    (destructor) _rule_dealloc, /* tp_dealloc */
    0,                          /* tp_print */
    0,                          /* tp_getattr */
    0,                          /* tp_setattr */
    0,                          /* tp_compare */
    0,                          /* tp_repr */
    0,                          /* tp_as_number */
    0,                          /* tp_as_sequence */
    0,                          /* tp_as_mapping */
    0,                          /* tp_hash */
    0,                          /* tp_call */
    0,                          /* tp_str */
    0,                          /* tp_getattro */
    0,                          /* tp_setattro */
    0,                          /* tp_as_buffer */
    Py_TPFLAGS_DEFAULT | Py_TPFLAGS_BASETYPE,
    "",
    0,                          /* tp_traverse */
    0,                          /* tp_clear */
    0,                          /* tp_richcompare */
    0,                          /* tp_weaklistoffset */
    0,                          /* tp_iter */
    0,                          /* tp_iternext */
    _rule_methods,              /* tp_methods */
    0,                          /* tp_members */
    _rule_getsets,              /* tp_getset */
    0,                          /* tp_base */
    0,                          /* tp_dict */
    0,                          /* tp_descr_get */
    0,                          /* tp_descr_set */
    offsetof (PyRule, dict),    /* tp_dictoffset */
    (initproc)_rule_init,       /* tp_init */
    0,                          /* tp_alloc */
    0,                          /* tp_new */
    0,                          /* tp_free */
    0,                          /* tp_is_gc */
    0,                          /* tp_bases */
    0,                          /* tp_mro */
    0,                          /* tp_cache */
    0,                          /* tp_subclasses */
    0,                          /* tp_weaklist */
    0,                          /* tp_del */
#if PY_VERSION_HEX >= 0x02060000
    0                           /* tp_version_tag */
#endif
};



/**
 * Creates a new PyRule.
 */
static int
_rule_init (PyObject *rule, PyObject *args, PyObject *kwds)
{
    PyObject *id;

    if (!PyArg_ParseTuple (args, "O", &id))
        return -1;

    Py_INCREF (id);
    ((PyRule*) rule)->id = id;
    ((PyRule*) rule)->weight = 0;
    ((PyRule*) rule)->code = NULL;
    return 0;
}

/**
 * Deallocates the PyRule.
 */
static void
_rule_dealloc (PyRule *rule)
{
    Py_XDECREF (rule->code);
    Py_XDECREF (rule->dict);
    Py_XDECREF (rule->id);
    rule->code = NULL;
    ((PyObject *)rule)->ob_type->tp_free ((PyObject *) rule);
}

/* Getters/Setters */
static PyObject*
_rule_getdict (PyRule *rule, void *closure)
{
    if (!rule->dict)
    {
        rule->dict = PyDict_New ();
        if (!rule->dict)
            return NULL;
    }

    Py_INCREF (rule->dict);
    return rule->dict;
}

static PyObject*
_rule_getid (PyRule *rule, void *closure)
{
    Py_INCREF (rule->id);
    return rule->id;
}

static PyObject*
_rule_getweight (PyRule *rule, void *closure)
{
    return PyFloat_FromDouble (rule->weight);
}

static int
_rule_setweight (PyRule *rule, PyObject *value, void *closure)
{
    double val;
    if (!get_double_from_obj (value, &val))
        return -1;
    if (val < 0)
    {
        PyErr_SetString (PyExc_ValueError, "weight must not be negative");
        return -1;
    }
    rule->weight = val;
    return 0;
}

static PyObject*
_rule_getcode (PyRule *rule, void *closure)
{
    if (rule->code)
    {
        Py_INCREF (rule->code);
        return rule->code;
    }
    Py_RETURN_NONE;
}

static int
_rule_setcode (PyRule *rule, PyObject *value, void *closure)
{
    if (rule->code)
    {
        Py_DECREF (rule->code);
    }

    if (value == Py_None)
        rule->code = NULL;
    else
    {
        Py_INCREF (value);
        rule->code = value;
    }
    return 0;
}

static PyObject*
_rule_getused (PyRule *rule, void *closure)
{
    return PyBool_FromLong (rule->used);
}

static int
_rule_setused (PyRule *rule, PyObject *value, void *closure)
{
    int val = PyObject_IsTrue (value);

    if (val == -1)
        return -1;
    rule->used = val;
    return 0;
}

/* C API */
static PyObject*
PyRule_New (PyObject *id)
{
    PyRule *rule;
    if (!id)
    {
        PyErr_SetString (PyExc_ValueError, "id must not be NULL");
        return NULL;
    }
    rule = (PyRule*) PyObject_New (PyRule, &PyRule_Type)
    if (!rule)
        return NULL;

    rule->dict = NULL;
    Py_INCREF (id);
    rule->id = id;
    rule->used = 0;
    rule->weight = 0;
    rule->code = NULL;
    return (PyObject*) rule;
}

void
rule_export_capi (void **capi)
{
    capi[DYNRULES_RULE_FIRSTSLOT+0] = &PyRule_Type;
    capi[DYNRULES_RULE_FIRSTSLOT+1] = PyRule_New;
}
