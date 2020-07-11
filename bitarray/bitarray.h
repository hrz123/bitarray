/*
   Copyright (c) 2008 - 2020, Ilan Schnell
   bitarray is published under the PSF license.

   This file is the C header file, which defines the bitarray C-API.

   Author: Ilan Schnell
*/
#ifndef bitarray_h
#define bitarray_h

#define PY_SSIZE_T_CLEAN
#include "Python.h"

#if PY_MAJOR_VERSION >= 3
#define IS_PY3K
#endif

#ifdef IS_PY3K
#define Py_TPFLAGS_HAVE_WEAKREFS  0
#endif

#if PY_MAJOR_VERSION == 3 || (PY_MAJOR_VERSION == 2 && PY_MINOR_VERSION == 7)
/* (new) buffer protocol */
#define WITH_BUFFER
#endif

#ifdef STDC_HEADERS
#include <stddef.h>
#else  /* !STDC_HEADERS */
#ifdef HAVE_SYS_TYPES_H
#include <sys/types.h>      /* For size_t */
#endif /* HAVE_SYS_TYPES_H */
#endif /* !STDC_HEADERS */


typedef long long int idx_t;

/* bit endianness */
#define ENDIAN_LITTLE  0
#define ENDIAN_BIG     1
static int default_endian = ENDIAN_BIG;

/* Unlike the normal convention, ob_size is the byte count, not the number
   of elements.  The reason for doing this is that we can use our own
   special idx_t for the number of bits, which may exceed 2^32 on a 32 bit
   machine.  */
typedef struct {
    PyObject_VAR_HEAD
    char *ob_item;
    Py_ssize_t allocated;       /* how many bytes allocated */
    idx_t nbits;                /* length of bitarray, i.e. elements */
    int endian;                 /* bit endianness of bitarray */
    int ob_exports;             /* how many buffer exports */
    PyObject *weakreflist;      /* list of weak references */
} bitarrayobject;

static PyTypeObject Bitarraytype;

#define ENDIAN_STR(a)  (((a)->endian == ENDIAN_LITTLE) ? "little" : "big")

#define BITS(bytes)  ((idx_t) (bytes) << 3)

/* number of bytes necessary to store given bits */
#define BYTES(bits)  (((bits) == 0) ? 0 : (((bits) - 1) / 8 + 1))

#define BITMASK(endian, i)  \
    (((char) 1) << ((endian) == ENDIAN_LITTLE ? ((i) % 8) : (7 - (i) % 8)))

/* ------------ low level access to bits in bitarrayobject ------------- */

#ifndef NDEBUG
static int GETBIT(bitarrayobject *self, idx_t i) {
    assert(0 <= i && i < self->nbits);
    return ((self)->ob_item[(i) / 8] & BITMASK((self)->endian, i) ? 1 : 0);
}
#else
#define GETBIT(self, i)  \
    ((self)->ob_item[(i) / 8] & BITMASK((self)->endian, i) ? 1 : 0)
#endif

void setbit(bitarrayobject *self, idx_t i, int bit);

int bitarray_Check(PyObject *obj);


#endif  /* bitarray_h */
