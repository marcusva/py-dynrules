#ifndef _COMPAT_H_
#define _COMPAT_H_

#include <Python.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Python 2.4 compatibility */
#if PY_VERSION_HEX < 0x02050000
typedef int Py_ssize_t;
#define PY_SSIZE_T_MAX INT_MAX
#define PY_SSIZE_T_MIN INT_MIN

typedef inquiry lenfunc;
typedef intargfunc ssizeargfunc;
typedef intobjargproc ssizeobjargproc;
typedef intintargfunc ssizessizeargfunc;
typedef intintobjargproc ssizessizeobjargproc;
typedef getreadbufferproc readbufferproc;
typedef getwritebufferproc writebufferproc;
typedef getsegcountproc segcountproc;
typedef getcharbufferproc charbufferproc;

#define PyIndex_Check(op) 0
#define PyInt_FromSsize_t(x) (PyInt_FromLong(x))

#endif /* PY_VERSION_HEX < 0x02050000 */

#if PY_VERSION_HEX >= 0x03000000

#define MOD_RETURN(x) return(x)
#define TYPE_HEAD(x,y) PyVarObject_HEAD_INIT(x,y)
#define IsFileObj(x) (!PyNumber_Check(x) && PyObject_AsFileDescriptor(x) != -1)

#else 

#define MOD_RETURN(x) return
#define TYPE_HEAD(x,y)                          \
    PyObject_HEAD_INIT(x)                       \
    0,

#define IsFileObj(x) PyFile_Check(x)

#endif /* PY_VERSION_HEX >= 0x03000000 */

#ifdef __cplusplus
}
#endif

#endif /* _COMPAT_H_ */
