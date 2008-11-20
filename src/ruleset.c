/*
 * dynrules - Python dynamic rules engine
 *
 * Authors: Marcus von Appen
 *
 * This file is distributed under the Public Domain.
 */

#define DYNRULES_RULESET_INTERNAL

#include "rulesmod.h"
#include "dynrules.h"

static int _ruleset_init (PyObject *ruleset, PyObject *args, PyObject *kwds);
static void _ruleset_dealloc (PyRuleSet *ruleset);

/* Getters/Setters */
static PyObject* _ruleset_getdict (PyRuleSet *rule, void *closure);
static PyObject* _ruleset_getweight (PyRuleSet *ruleset, void *closure);
static PyObject* _ruleset_getminweight (PyRuleSet *ruleset, void *closure);
static int _ruleset_setminweight (PyRuleSet *ruleset, PyObject *value,
    void *closure);
static PyObject* _ruleset_getmaxweight (PyRuleSet *ruleset, void *closure);
static int _ruleset_setmaxweight (PyRuleSet *ruleset, PyObject *value,
    void *closure);
static PyObject* _ruleset_getrules (PyRuleSet *ruleset, void *closure);

/* Methods */
static PyObject* _ruleset_clear (PyRuleSet *ruleset);
static PyObject* _ruleset_find (PyRuleSet *ruleset, PyObject *args);
static PyObject* _ruleset_addrule (PyRuleSet *ruleset, PyObject *args);
static PyObject* _ruleset_removerule (PyRuleSet *ruleset, PyObject *args);
static PyObject* _ruleset_updateweights (PyRuleSet *ruleset, PyObject *args);
static PyObject* _ruleset_calculateadjustment (PyRuleSet *ruleset,
    PyObject *args);
static PyObject* _ruleset_distributeremainder (PyRuleSet *ruleset,
    PyObject *args);

/* C API */
static PyObject* PyRuleSet_New (double minweight, double maxweight);
static int PyRuleSet_Add (PyObject *ruleset, PyObject *rule);
static int PyRuleSet_Remove (PyObject *ruleset, PyObject *rule);
static int PyRuleSet_UpdateWeights (PyObject *ruleset, PyObject *fitness);


/**
 * Methods bound to the PyRuleSet type.
 */
static PyMethodDef _ruleset_methods[] =
{
    { "clear", (PyCFunction)_ruleset_clear, METH_NOARGS, "" },
    { "add", (PyCFunction)_ruleset_addrule, METH_VARARGS, "" },
    { "find", (PyCFunction)_ruleset_find, METH_VARARGS, "" },
    { "remove", (PyCFunction)_ruleset_removerule, METH_VARARGS, "" },
    { "update_weights", (PyCFunction)_ruleset_updateweights, METH_VARARGS, "" },
    { "calculate_adjustment", (PyCFunction)_ruleset_calculateadjustment,
      METH_VARARGS, "" },
    { "distribute_remainder", (PyCFunction)_ruleset_distributeremainder,
      METH_VARARGS, "" },
    { NULL, NULL, 0, NULL }
};

/**
 * Getters and setters for the PyRuleSet type.
 */
static PyGetSetDef _ruleset_getsets[] =
{
    { "__dict__", (getter) _ruleset_getdict, NULL, NULL, NULL },
    { "weight", (getter) _ruleset_getweight, NULL, "", NULL },
    { "minweight", (getter) _ruleset_getminweight,
      (setter) _ruleset_setminweight, "", NULL },
    { "maxweight", (getter) _ruleset_getmaxweight,
      (setter) _ruleset_setmaxweight, "", NULL },
    { "rules", (getter) _ruleset_getrules, NULL, "", NULL },
    { NULL, NULL, NULL, NULL, NULL }
};

PyTypeObject PyRuleSet_Type =
{
    PyObject_HEAD_INIT(NULL)
    0,
    "_dynrules.RuleSet",           /* tp_name */
    sizeof (PyRuleSet),         /* tp_basicsize */
    0,                          /* tp_itemsize */
    (destructor) _ruleset_dealloc, /* tp_dealloc */
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
    _ruleset_methods,           /* tp_methods */
    0,                          /* tp_members */
    _ruleset_getsets,           /* tp_getset */
    0,                          /* tp_base */
    0,                          /* tp_dict */
    0,                          /* tp_descr_get */
    0,                          /* tp_descr_set */
    offsetof (PyRuleSet, dict), /* tp_dictoffset */
    (initproc)_ruleset_init,    /* tp_init */
    0,                          /* tp_alloc */
    0,                          /* tp_new */
    0,                          /* tp_free */
    0,                          /* tp_is_gc */
    0,                          /* tp_bases */
    0,                          /* tp_mro */
    0,                          /* tp_cache */
    0,                          /* tp_subclasses */
    0,                          /* tp_weaklist */
    0                           /* tp_del */
};



/**
 * Creates a new PyRuleSet.
 */
static int
_ruleset_init (PyObject *ruleset, PyObject *args, PyObject *kwds)
{
    int minw, maxw;

    if (!PyArg_ParseTuple (args, "ii", &minw, &maxw))
        return -1;

    ((PyRuleSet*) ruleset)->minweight = minw;
    ((PyRuleSet*) ruleset)->maxweight = maxw;
    ((PyRuleSet*) ruleset)->weight = 0;
    ((PyRuleSet*) ruleset)->rules = PyDict_New ();
    if (!((PyRuleSet*) ruleset)->rules)
        return -1;
    return 0;
}

/**
 * Deallocates the PyRuleSet.
 */
static void
_ruleset_dealloc (PyRuleSet *ruleset)
{
    Py_XDECREF (ruleset->rules);
    Py_XDECREF (ruleset->dict);
    ruleset->ob_type->tp_free ((PyObject *) ruleset);
}

/* Getters/Setters */
static PyObject*
_ruleset_getdict (PyRuleSet *ruleset, void *closure)
{
    if (!ruleset->dict)
    {
        ruleset->dict = PyDict_New ();
        if (!ruleset->dict)
            return NULL;
    }

    Py_INCREF (ruleset->dict);
    return ruleset->dict;
}

static PyObject*
_ruleset_getweight (PyRuleSet *ruleset, void *closure)
{
    return PyFloat_FromDouble (ruleset->weight);
}

static PyObject*
_ruleset_getminweight (PyRuleSet *ruleset, void *closure)
{
    return PyFloat_FromDouble (ruleset->minweight);
}

static int
_ruleset_setminweight (PyRuleSet *ruleset, PyObject *value, void *closure)
{
    double val;
    if (!get_double_from_obj (value, &val))
        return -1;
    if (val < 0)
    {
        PyErr_SetString (PyExc_ValueError, "minweight must not be negative");
        return -1;
    }
    if (val > ruleset->maxweight)
    {
        PyErr_SetString (PyExc_ValueError,
            "minweight must be smaller or equal to the set maxweight.");
        return -1;
    }
    ruleset->minweight = val;
    return 0;
}

static PyObject*
_ruleset_getmaxweight (PyRuleSet *ruleset, void *closure)
{
    return PyFloat_FromDouble (ruleset->maxweight);
}

static int
_ruleset_setmaxweight (PyRuleSet *ruleset, PyObject *value, void *closure)
{
    double val;
    if (!get_double_from_obj (value, &val))
        return -1;
    if (val < 0)
    {
        PyErr_SetString (PyExc_ValueError, "maxweight must not be negative");
        return -1;
    }
    if (val < ruleset->minweight)
    {
        PyErr_SetString (PyExc_ValueError,
            "maxweight must be greater or equal to the set minweight.");
        return -1;
    }

    ruleset->maxweight = val;
    return 0;
}

static PyObject*
_ruleset_getrules (PyRuleSet *ruleset, void *closure)
{
    return PyDict_Values (ruleset->rules);
}

/* Methods */
static PyObject*
_ruleset_clear (PyRuleSet *ruleset)
{
    PyDict_Clear (ruleset->rules);
    ruleset->weight = 0;
    Py_RETURN_NONE;
}

static PyObject*
_ruleset_find (PyRuleSet *ruleset, PyObject *args)
{
    PyObject *item, *key;
    
    if (!PyArg_ParseTuple (args, "O", &key))
        return NULL;
    
    item = PyDict_GetItem (ruleset->rules, key);
    if (item)
    {
        Py_INCREF (item);
        return item;
    }
    Py_RETURN_NONE;
}

static PyObject*
_ruleset_addrule (PyRuleSet *ruleset, PyObject *args)
{
    PyObject *rule;
    if (!PyArg_ParseTuple (args, "O:add", &rule))
        return NULL;
    if (!PyRuleSet_Add ((PyObject*)ruleset, rule))
        return NULL;
    Py_RETURN_NONE;
}

static PyObject*
_ruleset_removerule (PyRuleSet *ruleset, PyObject *args)
{
    PyObject *rule;
    if (!PyArg_ParseTuple (args, "O:remove", &rule))
        return NULL;
    if (!PyRuleSet_Remove ((PyObject*)ruleset, rule))
        return NULL;
    Py_RETURN_NONE;
}

static PyObject*
_ruleset_updateweights (PyRuleSet *ruleset, PyObject *args)
{
    PyObject *fraction;

    if (!PyArg_ParseTuple (args, "O:update_weights", &fraction))
        return NULL;
    if (!PyRuleSet_UpdateWeights ((PyObject*) ruleset, fraction))
        return NULL;
    Py_RETURN_NONE;
}

static PyObject*
_ruleset_calculateadjustment (PyRuleSet *ruleset, PyObject *args)
{
    PyErr_SetString (PyExc_NotImplementedError, "method not implemented");
    return NULL;
}

static PyObject*
_ruleset_distributeremainder (PyRuleSet *ruleset, PyObject *args)
{
    PyErr_SetString (PyExc_NotImplementedError, "method not implemented");
    return NULL;
}

/* C API */
static PyObject*
PyRuleSet_New (double minweight, double maxweight)
{
    PyRuleSet *ruleset = (PyRuleSet*) PyObject_New (PyRuleSet, &PyRuleSet_Type);
    if (!ruleset)
        return NULL;

    ruleset->dict = NULL;
    ruleset->weight = 0;
    ruleset->minweight = minweight;
    ruleset->maxweight = maxweight;
    ruleset->rules = PyDict_New ();
    if (!ruleset->rules)
    {
        Py_DECREF (ruleset);
        return NULL;
    }
    return (PyObject*) ruleset;
}

static int
PyRuleSet_Add (PyObject *ruleset, PyObject *rule)
{
    PyRuleSet *rset;
    PyRule *r, *entry;
    PyObject *kv;

    if (!PyRuleSet_Check (ruleset))
    {
        PyErr_SetString (PyExc_TypeError, "ruleset must be a RuleSet");
        return 0;
    }

    if (!PyRule_Check (rule))
    {
        PyErr_SetString (PyExc_TypeError, "rule must be a Rule");
        return 0;
    }

    rset = (PyRuleSet*) ruleset;
    r = (PyRule*) rule;

    kv = PyInt_FromLong (r->id);

    entry = (PyRule*) PyDict_GetItem (rset->rules, kv);
    if (entry)
        rset->weight -= entry->weight;

    if (PyDict_SetItem (rset->rules, kv, rule) == -1)
    {
        Py_DECREF (kv);
        return 0;
    }
    Py_DECREF (kv);

    if (r->weight > rset->maxweight)
        r->weight = rset->maxweight;
    else if (r->weight < rset->minweight)
        r->weight = rset->minweight;

    rset->weight += r->weight;
    return 1;
}

static int
PyRuleSet_Remove (PyObject *ruleset, PyObject *rule)
{
    PyRuleSet *rset;
    PyRule *r, *entry;
    PyObject *kv;

    if (!PyRuleSet_Check (ruleset))
    {
        PyErr_SetString (PyExc_TypeError, "ruleset must be a RuleSet");
        return 0;
    }

    if (!PyRule_Check (rule))
    {
        PyErr_SetString (PyExc_TypeError, "rule must be a Rule");
        return 0;
    }
    rset = (PyRuleSet*) ruleset;
    r = (PyRule*) rule;

    kv = PyInt_FromLong (r->id);

    entry = (PyRule*) PyDict_GetItem (rset->rules, kv);
    if (entry)
    {
        /* An entry exists, compare it. */
        int cmp = PyObject_RichCompareBool (rule, (PyObject*) entry, Py_EQ);
        if (cmp == -1)
        {
            Py_DECREF (kv);
            return 0;
        }
        else if (cmp == 0)
        {
            Py_DECREF (kv);
            PyErr_SetString (PyExc_ValueError,
                "rule does not match rule in RuleSet");
            return 0;
        }
        /* Anything's okay. */
    }
    else
    {
        Py_DECREF (kv);
        PyErr_SetString (PyExc_ValueError, "rule does not exist");
        return 0;
    }

    if (PyDict_DelItem (rset->rules, kv) == -1)
    {
        Py_DECREF (kv);
        return 0;
    }
    Py_DECREF (kv);
    rset->weight -= r->weight;
    return 1;
}

static int
PyRuleSet_UpdateWeights (PyObject *ruleset, PyObject *fitness)
{
    /*
     * Adapted from Pieter Spronck's algorithm as explained in
     * Spronck et al: 2005, 'Adaptive Game AI with Dynamic Scripting'
     */

    PyRuleSet *rset;
    PyRule *rule;
    PyObject *rules, *adjust, *retval;
    Py_ssize_t usedcount = 0, count, i;
    int nonactive;
    double totweight = 0, adjustment, compensation, _remainder;

    if (!PyRuleSet_Check (ruleset))
    {
        PyErr_SetString (PyExc_TypeError, "ruleset must be a RuleSet");
        return 0;
    }
    rset = (PyRuleSet*) ruleset;
    rules = PyDict_Values (rset->rules);
    if (!rules)
        return 0;

    count = PyList_Size (rules);
    if (count == 0)
    {
        Py_DECREF (rules);
        return 1;
    }

    for (i = 0; i < count; i++)
    {
        rule = (PyRule*) PyList_GET_ITEM (rules, i);
        if (rule->used)
            usedcount++;
    }

    if (usedcount == 0 || usedcount == count)
    {
        Py_DECREF (rules);
        return 1;
    }

    nonactive = count - usedcount;

    adjust = PyObject_CallMethod (ruleset, "calculate_adjustment", "O",
        fitness, NULL);
    if (!adjust)
    {
        Py_DECREF (rules);
        return 0;
    }
    if (!get_double_from_obj (adjust, &adjustment))
    {
        Py_DECREF (rules);
        return 0;
    }
    Py_DECREF (adjust);

    compensation = ((double) (-usedcount * adjustment)) / nonactive;
    _remainder = 0;

    for (i = 0; i < count; i++)
    {
        rule = (PyRule*) PyList_GET_ITEM (rules, i);
        rule->weight += (rule->used) ? adjustment : compensation;
        if (rule->weight < rset->minweight)
        {
            _remainder += rule->weight - rset->minweight;
            rule->weight = rset->minweight;
        }
        else if (rule->weight > rset->maxweight)
        {
            _remainder += rule->weight - rset->maxweight;
            rule->weight = rset->maxweight;
        }
        totweight += rule->weight;
    }

    rset->weight = totweight;

    Py_DECREF (rules);

    retval = PyObject_CallMethod (ruleset, "distribute_remainder", "d",
        _remainder);
    if (!retval)
        return 0;
    Py_DECREF (retval);

    totweight = 0;
    for (i = 0; i < count; i++)
    {
        rule = (PyRule*) PyList_GET_ITEM (rules, i);
        totweight += rule->weight;
    }
    rset->weight = totweight;

    return 1;
}

void
ruleset_export_capi (void **capi)
{
    capi[DYNRULES_RULESET_FIRSTSLOT+0] = &PyRuleSet_Type;
    capi[DYNRULES_RULESET_FIRSTSLOT+1] = PyRuleSet_New;
    capi[DYNRULES_RULESET_FIRSTSLOT+2] = PyRuleSet_Add;
    capi[DYNRULES_RULESET_FIRSTSLOT+3] = PyRuleSet_Remove;
    capi[DYNRULES_RULESET_FIRSTSLOT+4] = PyRuleSet_UpdateWeights;
}
