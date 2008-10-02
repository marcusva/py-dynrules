#ifndef _COMPAT_H_
#define _COMPAT_H_

#include <Python.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Python 2.4 compatibility */
#if PY_VERSION_HEX < 0x02050000 && !defined(PY_SSIZE_T_MIN)
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

#endif /* PY_VERSION_HEX < 0x02050000 && !defined(PY_SSIZE_T_MIN) */

#ifdef __cplusplus
}
#endif

#endif /* _COMPAT_H_ */
