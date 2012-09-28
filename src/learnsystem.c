/*
 * dynrules - Python dynamic rules engine
 *
 * Authors: Marcus von Appen
 *
 * This file is distributed under the Public Domain.
 */

#define DYNRULES_LEARNSYSTEM_INTERNAL

#include <stdlib.h>
#include <time.h>
#include "rulesmod.h"
#include "dynrules.h"

static int _lsystem_init (PyObject *lsystem, PyObject *args, PyObject *kwds);
static void _lsystem_dealloc (PyLearnSystem *lsystem);

/* Getters/Setters */
static PyObject* _lsystem_getdict (PyLearnSystem *lsystem, void *closure);
static PyObject* _lsystem_getmaxtries (PyLearnSystem *lsystem, void *closure);
static int _lsystem_setmaxtries (PyLearnSystem *lsystem, PyObject *value,
    void *closure);
static PyObject* _lsystem_getmaxscriptsize (PyLearnSystem *lsystem,
    void *closure);
static int _lsystem_setmaxscriptsize (PyLearnSystem *lsystem, PyObject *value,
    void *closure);
static PyObject* _lsystem_getruleset (PyLearnSystem *lsystem, void *closure);
static int _lsystem_setruleset (PyLearnSystem *lsystem, PyObject *value,
    void *closure);

/* Methods */
static PyObject* _lsystem_create_script (PyLearnSystem *lsystem,
    PyObject *args);
static PyObject* _lsystem_create_rules (PyLearnSystem *lsystem, PyObject *args);
static PyObject* _lsystem_create_header (PyLearnSystem *lsystem);
static PyObject* _lsystem_create_footer (PyLearnSystem *lsystem);

/* C API */
static PyObject* PyLearnSystem_New (PyObject *ruleset);
static int PyLearnSystem_CreateScript (PyObject *lsystem, PyObject *file,
    int maxrules);

/**
 * Methods bound to the PyLearnSystem type.
 */
static PyMethodDef _lsystem_methods[] =
{
    { "create_header", (PyCFunction)_lsystem_create_header, METH_NOARGS, "" },
    { "create_footer", (PyCFunction)_lsystem_create_footer, METH_NOARGS, "" },
    { "create_rules", (PyCFunction)_lsystem_create_rules, METH_VARARGS, "" },
    { "create_script", (PyCFunction)_lsystem_create_script, METH_VARARGS, "" },
    { NULL, NULL, 0, NULL }
};

/**
 * Getters and setters for the PyLearnSystem type.
 */
static PyGetSetDef _lsystem_getsets[] =
{
    { "__dict__", (getter) _lsystem_getdict, NULL, NULL, NULL },
    { "maxtries", (getter) _lsystem_getmaxtries, (setter) _lsystem_setmaxtries,
      "", NULL },
    { "maxscriptsize", (getter) _lsystem_getmaxscriptsize,
      (setter) _lsystem_setmaxscriptsize, "", NULL },
    { "ruleset", (getter) _lsystem_getruleset, (setter) _lsystem_setruleset,
      "", NULL },
    { NULL, NULL, NULL, NULL, NULL }
};

PyTypeObject PyLearnSystem_Type =
{
    TYPE_HEAD(NULL,0)
    "_dynrules.LearnSystem",    /* tp_name */
    sizeof (PyLearnSystem),     /* tp_basicsize */
    0,                          /* tp_itemsize */
    (destructor) _lsystem_dealloc, /* tp_dealloc */
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
    _lsystem_methods,           /* tp_methods */
    0,                          /* tp_members */
    _lsystem_getsets,           /* tp_getset */
    0,                          /* tp_base */
    0,                          /* tp_dict */
    0,                          /* tp_descr_get */
    0,                          /* tp_descr_set */
    offsetof (PyLearnSystem, dict), /* tp_dictoffset */
    (initproc)_lsystem_init,    /* tp_init */
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
 * Creates a new PyLearnSystem.
 */
static int
_lsystem_init (PyObject *lsystem, PyObject *args, PyObject *kwds)
{
    PyObject *ruleset;

    if (!PyArg_ParseTuple (args, "O", &ruleset))
        return -1;

    if (!PyRuleSet_Check (ruleset))
    {
        PyErr_SetString (PyExc_TypeError, "ruleset must be a RuleSet");
        return -1;
    }

    ((PyLearnSystem*) lsystem)->maxtries = 100;
    ((PyLearnSystem*) lsystem)->maxscriptsize = 1024;
    Py_INCREF (ruleset);
    ((PyLearnSystem*) lsystem)->ruleset = ruleset;
    return 0;
}

/**
 * Deallocates the PyLearnSystem.
 */
static void
_lsystem_dealloc (PyLearnSystem *lsystem)
{
    Py_XDECREF (lsystem->ruleset);
    Py_XDECREF (lsystem->dict);
    lsystem->ruleset = NULL;
    ((PyObject *)lsystem)->ob_type->tp_free ((PyObject *) lsystem);
}

/* Getters/Setters */
static PyObject*
_lsystem_getdict (PyLearnSystem *lsystem, void *closure)
{
    if (!lsystem->dict)
    {
        lsystem->dict = PyDict_New ();
        if (!lsystem->dict)
            return NULL;
    }

    Py_INCREF (lsystem->dict);
    return lsystem->dict;
}

static PyObject*
_lsystem_getmaxtries (PyLearnSystem *lsystem, void *closure)
{
    return PyLong_FromLong (lsystem->maxtries);
}

static int
_lsystem_setmaxtries (PyLearnSystem *lsystem, PyObject *value, void *closure)
{
    int maxtries;

    if (!get_int_from_obj (value, &maxtries))
        return -1;
    if (maxtries <= 0)
    {
        PyErr_SetString (PyExc_ValueError, "maxtries must be greater than 0");
        return -1;
    }
    lsystem->maxtries = maxtries;
    return 0;
}

static PyObject*
_lsystem_getmaxscriptsize (PyLearnSystem *lsystem, void *closure)
{
    return PyLong_FromLong (lsystem->maxscriptsize);
}

static int
_lsystem_setmaxscriptsize (PyLearnSystem *lsystem, PyObject *value,
    void *closure)
{
    int maxsize;

    if (!get_int_from_obj (value, &maxsize))
        return -1;
    if (maxsize <= 0)
    {
        PyErr_SetString (PyExc_ValueError, "maxtries must be greater than 0");
        return -1;
    }
    lsystem->maxscriptsize = maxsize;
    return 0;
}

static PyObject*
_lsystem_getruleset (PyLearnSystem *lsystem, void *closure)
{
    Py_INCREF (lsystem->ruleset);
    return lsystem->ruleset;
}

static int
_lsystem_setruleset (PyLearnSystem *lsystem, PyObject *value, void *closure)
{
    if (!PyRuleSet_Check (value))
    {
        PyErr_SetString (PyExc_TypeError, "ruleset must be a RuleSet");
        return -1;
    }
    Py_DECREF (lsystem->ruleset);
    Py_INCREF (value);
    lsystem->ruleset = value;
    return 0;
}

/* Methods */
static PyObject*
_lsystem_create_script (PyLearnSystem *lsystem, PyObject *args)
{
    PyObject *file;
    int maxrules;

    if (!PyArg_ParseTuple (args, "Oi:create_script", &file, &maxrules))
        return NULL;
    if (!PyLearnSystem_CreateScript ((PyObject*)lsystem, file, maxrules))
        return NULL;
    Py_RETURN_NONE;
}

static PyObject*
_lsystem_create_rules (PyLearnSystem *lsystem, PyObject *args)
{
    double weights = ((PyRuleSet*)lsystem->ruleset)->weight;
    int added = 0, maxrules, i, j, tries, selected;
    Py_ssize_t count, written;
    PyObject *rules, *buf, *retval;
    PyRule *rule;
    double wsum, fraction;
#if PY_VERSION_HEX < 0x03000000 && !defined(PYPY_VERSION)
    struct PycStringIO_CAPI* stringio;
#else
    PyObject *tmpbuf;
#endif

    if (!PyArg_ParseTuple (args, "i:create_rules", &maxrules))
        return NULL;

    if (maxrules <= 0)
    {
        PyErr_SetString (PyExc_ValueError, "maxrules must be greater than 0");
        return NULL;
    }

    if (weights == 0)
        Py_RETURN_NONE;

#if PY_VERSION_HEX < 0x03000000 && !defined(PYPY_VERSION)
    stringio = get_stringio_api ();
    if (!stringio)
    {
        PyErr_SetString (PyExc_ValueError, "stringIO is NULL");
        return NULL;
    }
    buf = stringio->NewOutput (lsystem->maxscriptsize);
    if (!buf)
        return NULL;
#else
    /* Use a byte sequence. */
    buf = PyBytes_FromString ("");
    if (!buf)
        return NULL;
#endif /* PY_VERSION_HEX < 0x03000000 && !defined(PYPY_VERSION) */

    written = 0;

    rules = PyDict_Values (((PyRuleSet*)lsystem->ruleset)->rules);
    count = PyList_Size (rules);

    /* Initialise the random number generator */
    srand ((unsigned) time (NULL));

    /* TODO: we do not guarantee that j < count. Usually this should be
     * dealt by the fraction sum function, though a safety net using
     * j % count or so might be better.
     */
    for (i = 0; i < maxrules; i++)
    {
        if (written >= lsystem->maxscriptsize)
            break;

        tries = added = 0;
        while (tries < lsystem->maxtries && !added)
        {
            j = 0;
            selected = -1;
            wsum = 0.;
            fraction = (((double) rand()) / RAND_MAX) * weights;
            while (selected == -1)
            {
                rule = (PyRule*) PyList_GET_ITEM (rules, j);
                wsum += rule->weight;
                if (wsum > fraction)
                {
                    selected = j;
                    break;
                }
                j++;
            }

            rule = (PyRule*) PyList_GET_ITEM (rules, selected);

            /* Write the rule code */
            if (!rule->code)
            {
                /* No code given. */
                tries++;
                break;
            }
            if (PyObject_CheckReadBuffer (rule->code))
            {
                Py_ssize_t len;
                const void *charbuf;
                if (PyObject_AsReadBuffer (rule->code, &charbuf, &len) == 0)
                {
                    /* Buffer acquired, write the raw data. */
                    if (written + len > lsystem->maxscriptsize)
                        goto finish;

#if PY_VERSION_HEX < 0x03000000 && !defined(PYPY_VERSION)
                    stringio->cwrite (buf, (const char*)charbuf, len);
#else
                    tmpbuf = PyBytes_FromString (charbuf);
                    if (!tmpbuf)
                    {
                        Py_XDECREF (buf);
                        return NULL;
                    }
                    PyBytes_ConcatAndDel (&buf, tmpbuf);
#endif
                    written += len;
                    added = 1;
                }
                PyErr_Clear (); /* Clear any error, that occured before. */
            }

            if (!added)
            {
                /* No buffer - try the str(obj) representation */
                Py_ssize_t len;
                char *charbuf;
#if PY_VERSION_HEX < 0x03000000
                PyObject *str = PyObject_Str (rule->code);
#else
                PyObject *str = PyObject_Bytes (rule->code);
#endif
                if (str)
                {
#if PY_VERSION_HEX < 0x03000000
                    len = PyString_GET_SIZE (str);
#else
                    len = PyBytes_GET_SIZE (str);
#endif
                    if (written + len > lsystem->maxscriptsize)
                        goto finish;

                    /* Write it */
#if PY_VERSION_HEX < 0x03000000
                    charbuf = PyString_AsString (str);
#else
                    charbuf = PyBytes_AS_STRING (str);
#endif
                    if (charbuf)
                    {
                        added = 1;
#if PY_VERSION_HEX < 0x03000000 && !defined(PYPY_VERSION)
                        stringio->cwrite (buf, charbuf, len);
#else
                        tmpbuf = PyBytes_FromString (charbuf);
                        if (!tmpbuf)
                        {
                            Py_XDECREF (buf);
                            return NULL;
                        }
#endif
                        written += len;
                    }
                }
                PyErr_Clear (); /* Clear any error, that occured before. */
            }
            tries++;
            break;
        }
    }

finish:

#if PY_VERSION_HEX < 0x03000000 && !defined(PYPY_VERSION)
    retval = stringio->cgetvalue (buf);
#else
    retval = buf;
#endif
    return retval;

}

static PyObject*
_lsystem_create_header (PyLearnSystem *lsystem)
{
    Py_RETURN_NONE;
}

static PyObject*
_lsystem_create_footer (PyLearnSystem *lsystem)
{
    Py_RETURN_NONE;
}

/* C API */
static PyObject*
PyLearnSystem_New (PyObject *ruleset)
{
    PyLearnSystem *lsystem;

    if (!PyRuleSet_Check (ruleset))
    {
        PyErr_SetString (PyExc_TypeError, "ruleset must be a RuleSet");
        return NULL;
    }

    lsystem = (PyLearnSystem*) PyObject_New (PyLearnSystem,
        &PyLearnSystem_Type);
    if (!lsystem)
        return NULL;

    Py_INCREF (ruleset);
    lsystem->dict = NULL;
    lsystem->ruleset = ruleset;
    lsystem->maxtries = 100;
    lsystem->maxscriptsize = 1024;
    return (PyObject*) lsystem;
}

static int
PyLearnSystem_CreateScript (PyObject *lsystem, PyObject *file, int maxrules)
{
    int alreadyopen = 0;
    PyObject *fp, *header, *footer, *rules;
    Py_ssize_t hsize, fsize;

    if (maxrules <= 0)
    {
        PyErr_SetString (PyExc_ValueError, "maxrules must be greater than 0");
        return 0;
    }

    if (!PyLearnSystem_Check (lsystem))
    {
        PyErr_SetString (PyExc_TypeError, "lsystem must be a LearnSystem");
        return 0;
    }

    if (IsFileObj(file))
    {
        fp = file;
        alreadyopen = 1;
    }
#if PY_VERSION_HEX < 0x03000000
    else if (PyUnicode_Check (file) || PyString_Check (file))
    {
        char *s = PyString_AsString (file);
        if (!s)
            return 0;

        fp = PyFile_FromString (s, "a");
        if (!fp)
            return 0;
    }
#else
    else if (PyUnicode_Check (file) ||  PyBytes_Check (file))
    {
        PyErr_Clear (); /* PyObject_AsFileDescriptor() sets an exception */
        PyObject *io = PyImport_ImportModule ("io");
        if (!io)
            return 0;
        fp = PyObject_CallMethod (io, "open", "ss", file, "w");
        Py_DECREF (io);
        if (!fp)
            return 0;
    }
#endif /* PY_VERSION_HEX < 0x03000000 */
    else
    {
        PyErr_SetString (PyExc_TypeError,
            "file must be a file object or string");
        return 0;
    }

    /* Write the header */
    header = PyObject_CallMethod (lsystem, "create_header", NULL);
    if (!header)
        goto error;

    if (header != Py_None)
    {
        hsize = PyObject_Size (header);
        if (hsize == -1)
        {
            Py_DECREF (header);
            goto error;
        }

        if (PyFile_WriteObject (header, fp, Py_PRINT_RAW) == -1)
        {
            Py_DECREF (header);
            goto error;
            return 0;
        }
    }
    Py_DECREF (header);

    /* Write the rules */
    rules = PyObject_CallMethod (lsystem, "create_rules", "i", maxrules);
    if (!rules)
        goto error;

    if (rules != Py_None)
    {
        if (PyFile_WriteObject (rules, fp, Py_PRINT_RAW) == -1)
        {
            Py_DECREF (rules);
            goto error;
            return 0;
        }
    }
    Py_DECREF (rules);

    /* Write the footer */
    footer = PyObject_CallMethod (lsystem, "create_footer", NULL);
    if (footer != Py_None)
    {
        fsize = PyObject_Size (footer);
        if (fsize == -1)
        {
            Py_DECREF (footer);
            goto error;
        }

        if (PyFile_WriteObject (footer, fp, Py_PRINT_RAW) == -1)
        {
            Py_DECREF (footer);
            goto error;
        }
    }
    Py_DECREF (footer);

    if (!alreadyopen)
    {
        Py_DECREF (fp);
    }
    return 1;

error:
    if (!alreadyopen)
    {
        Py_DECREF (fp);
    }
    return 0;
}

void
learnsystem_export_capi (void **capi)
{
    capi[DYNRULES_LEARNSYSTEM_FIRSTSLOT+0] = &PyLearnSystem_Type;
    capi[DYNRULES_LEARNSYSTEM_FIRSTSLOT+1] = PyLearnSystem_New;
    capi[DYNRULES_LEARNSYSTEM_FIRSTSLOT+2] = PyLearnSystem_CreateScript;
}
