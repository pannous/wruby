/*
 ** pack.c - Array#pack, String#unpack
 */

#include "mruby.h"
#include "error.h"
#include "mruby/array.h"
#include "mruby/class.h"
#include "mruby/numeric.h"
#include "mruby/string.h"
#include "mruby/variable.h"

#include <ctype.h>
#include <errno.h>
#include <limits.h>
#include <stdio.h>
#include <string.h>

struct tmpl {
  _value str;
  int idx;
};

enum {
  PACK_DIR_CHAR,      /* C */
  PACK_DIR_SHORT,     /* S */
  PACK_DIR_LONG,      /* L */
  PACK_DIR_QUAD,      /* Q */
  //PACK_DIR_INT,     /* i */
  //PACK_DIR_VAX,
  PACK_DIR_UTF8,      /* U */
  //PACK_DIR_BER,
  PACK_DIR_DOUBLE,    /* E */
  PACK_DIR_FLOAT,     /* f */
  PACK_DIR_STR,       /* A */
  PACK_DIR_HEX,       /* h */
  PACK_DIR_BASE64,    /* m */
  PACK_DIR_NUL,       /* x */
  PACK_DIR_INVALID
};

enum {
  PACK_TYPE_INTEGER,
  PACK_TYPE_FLOAT,
  PACK_TYPE_STRING,
  PACK_TYPE_NONE
};

#define PACK_FLAG_s             0x00000001	/* native size ("_" "!") */
#define PACK_FLAG_a             0x00000002	/* null padding ("a") */
#define PACK_FLAG_Z             0x00000004	/* append nul char ("z") */
#define PACK_FLAG_SIGNED        0x00000008	/* native size ("_" "!") */
#define PACK_FLAG_GT            0x00000010	/* big endian (">") */
#define PACK_FLAG_LT            0x00000020	/* little endian ("<") */
#define PACK_FLAG_WIDTH         0x00000040	/* "count" is "width" */
#define PACK_FLAG_LSB           0x00000080	/* LSB / low nibble first */
#define PACK_FLAG_COUNT2        0x00000100	/* "count" is special... */
#define PACK_FLAG_LITTLEENDIAN  0x00000200	/* little endian actually */

#define PACK_BASE64_IGNORE	0xff
#define PACK_BASE64_PADDING	0xfe

static int littleendian = 0;

const static unsigned char base64chars[] =
    "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
static unsigned char base64_dec_tab[128];


static int
check_little_endian(void)
{
  unsigned int n = 1;
  return (*(unsigned char *)&n == 1);
}

static unsigned int
hex2int(unsigned char ch)
{
  if (ch >= '0' && ch <= '9')
    return ch - '0';
  else if (ch >= 'A' && ch <= 'F')
    return 10 + (ch - 'A');
  else if (ch >= 'a' && ch <= 'f')
    return 10 + (ch - 'a');
  else
    return 0;
}

static void
make_base64_dec_tab(void)
{
  int i;
  memset(base64_dec_tab, PACK_BASE64_IGNORE, sizeof(base64_dec_tab));
  for (i = 0; i < 26; i++)
    base64_dec_tab['A' + i] = i;
  for (i = 0; i < 26; i++)
    base64_dec_tab['a' + i] = i + 26;
  for (i = 0; i < 10; i++)
    base64_dec_tab['0' + i] = i + 52;
  base64_dec_tab['+'] = 62;
  base64_dec_tab['/'] = 63;
  base64_dec_tab['='] = PACK_BASE64_PADDING;
}

static _value
str_len_ensure(_state *mrb, _value str, _int len)
{
  _int n = RSTRING_LEN(str);
  if (len < 0) {
    _raise(mrb, E_RANGE_ERROR, "negative (or overflowed) integer");
  }
  if (len > n) {
    do {
      n *= 2;
    } while (len > n);
    str = _str_resize(mrb, str, n);
  }
  return str;
}


static int
pack_c(_state *mrb, _value o, _value str, _int sidx, unsigned int flags)
{
  str = str_len_ensure(mrb, str, sidx + 1);
  RSTRING_PTR(str)[sidx] = (char)_fixnum(o);
  return 1;
}

static int
unpack_c(_state *mrb, const void *src, int srclen, _value ary, unsigned int flags)
{
  if (flags & PACK_FLAG_SIGNED)
    _ary_push(mrb, ary, _fixnum_value(*(signed char *)src));
  else
    _ary_push(mrb, ary, _fixnum_value(*(unsigned char *)src));
  return 1;
}

static int
pack_s(_state *mrb, _value o, _value str, _int sidx, unsigned int flags)
{
  uint16_t n;

  str = str_len_ensure(mrb, str, sidx + 2);
  n = (uint16_t)_fixnum(o);
  if (flags & PACK_FLAG_LITTLEENDIAN) {
    RSTRING_PTR(str)[sidx+0] = n % 256;
    RSTRING_PTR(str)[sidx+1] = n / 256;
  } else {
    RSTRING_PTR(str)[sidx+0] = n / 256;
    RSTRING_PTR(str)[sidx+1] = n % 256;
  }
  return 2;
}

static int
unpack_s(_state *mrb, const unsigned char *src, int srclen, _value ary, unsigned int flags)
{
  int n;

  if (flags & PACK_FLAG_LITTLEENDIAN) {
    n = src[1] * 256 + src[0];
  } else {
    n = src[0] * 256 + src[1];
  }
  if ((flags & PACK_FLAG_SIGNED) && (n >= 0x8000)) {
    n -= 0x10000;
  }
  _ary_push(mrb, ary, _fixnum_value(n));
  return 2;
}

static int
pack_l(_state *mrb, _value o, _value str, _int sidx, unsigned int flags)
{
  uint32_t n;

  str = str_len_ensure(mrb, str, sidx + 4);
  n = (uint32_t)_fixnum(o);
  if (flags & PACK_FLAG_LITTLEENDIAN) {
    RSTRING_PTR(str)[sidx+0] = (char)(n & 0xff);
    RSTRING_PTR(str)[sidx+1] = (char)(n >> 8);
    RSTRING_PTR(str)[sidx+2] = (char)(n >> 16);
    RSTRING_PTR(str)[sidx+3] = (char)(n >> 24);
  } else {
    RSTRING_PTR(str)[sidx+0] = (char)(n >> 24);
    RSTRING_PTR(str)[sidx+1] = (char)(n >> 16);
    RSTRING_PTR(str)[sidx+2] = (char)(n >> 8);
    RSTRING_PTR(str)[sidx+3] = (char)(n & 0xff);
  }
  return 4;
}

static int
unpack_l(_state *mrb, const unsigned char *src, int srclen, _value ary, unsigned int flags)
{
#ifndef MRB_INT64
  char msg[60];
#endif
  uint32_t ul;
  _int n;

  if (flags & PACK_FLAG_LITTLEENDIAN) {
    ul = (uint32_t)src[3] * 256*256*256;
    ul += (uint32_t)src[2] *256*256;
    ul += (uint32_t)src[1] *256;
    ul += (uint32_t)src[0];
  } else {
    ul = (uint32_t)src[0] * 256*256*256;
    ul += (uint32_t)src[1] *256*256;
    ul += (uint32_t)src[2] *256;
    ul += (uint32_t)src[3];
  }
  if (flags & PACK_FLAG_SIGNED) {
    int32_t sl = ul;
#ifndef MRB_INT64
    if (!FIXABLE(sl)) {
      snprintf(msg, sizeof(msg), "cannot unpack to Fixnum: %ld", (long)sl);
      _raise(mrb, E_RANGE_ERROR, msg);
    }
#endif
    n = sl;
  } else {
#ifndef MRB_INT64
    if (!POSFIXABLE(ul)) {
      snprintf(msg, sizeof(msg), "cannot unpack to Fixnum: %lu", (unsigned long)ul);
      _raise(mrb, E_RANGE_ERROR, msg);
    }
#endif
    n = ul;
  }
  _ary_push(mrb, ary, _fixnum_value(n));
  return 4;
}

static int
pack_q(_state *mrb, _value o, _value str, _int sidx, unsigned int flags)
{
  uint64_t n;

  str = str_len_ensure(mrb, str, sidx + 8);
  n = (uint64_t)_fixnum(o);
  if (flags & PACK_FLAG_LITTLEENDIAN) {
    RSTRING_PTR(str)[sidx+0] = (char)(n & 0xff);
    RSTRING_PTR(str)[sidx+1] = (char)(n >> 8);
    RSTRING_PTR(str)[sidx+2] = (char)(n >> 16);
    RSTRING_PTR(str)[sidx+3] = (char)(n >> 24);
    RSTRING_PTR(str)[sidx+4] = (char)(n >> 32);
    RSTRING_PTR(str)[sidx+5] = (char)(n >> 40);
    RSTRING_PTR(str)[sidx+6] = (char)(n >> 48);
    RSTRING_PTR(str)[sidx+7] = (char)(n >> 56);
  } else {
    RSTRING_PTR(str)[sidx+0] = (char)(n >> 56);
    RSTRING_PTR(str)[sidx+1] = (char)(n >> 48);
    RSTRING_PTR(str)[sidx+2] = (char)(n >> 40);
    RSTRING_PTR(str)[sidx+3] = (char)(n >> 32);
    RSTRING_PTR(str)[sidx+4] = (char)(n >> 24);
    RSTRING_PTR(str)[sidx+5] = (char)(n >> 16);
    RSTRING_PTR(str)[sidx+6] = (char)(n >> 8);
    RSTRING_PTR(str)[sidx+7] = (char)(n & 0xff);
  }
  return 8;
}

static int
unpack_q(_state *mrb, const unsigned char *src, int srclen, _value ary, unsigned int flags)
{
  char msg[60];
  uint64_t ull;
  int i, pos, step;
  _int n;

  if (flags & PACK_FLAG_LITTLEENDIAN) {
    pos  = 7;
    step = -1;
  } else {
    pos  = 0;
    step = 1;
  }
  ull = 0;
  for (i = 0; i < 8; i++) {
    ull = ull * 256 + (uint64_t)src[pos];
    pos += step;
  }
  if (flags & PACK_FLAG_SIGNED) {
    int64_t sll = ull;
    if (!FIXABLE(sll)) {
      snprintf(msg, sizeof(msg), "cannot unpack to Fixnum: %lld", (long long)sll);
      _raise(mrb, E_RANGE_ERROR, msg);
    }
    n = sll;
  } else {
    if (!POSFIXABLE(ull)) {
      snprintf(msg, sizeof(msg), "cannot unpack to Fixnum: %llu", (unsigned long long)ull);
      _raise(mrb, E_RANGE_ERROR, msg);
    }
    n = ull;
  }
  _ary_push(mrb, ary, _fixnum_value(n));
  return 8;
}

#ifndef MRB_WITHOUT_FLOAT
static int
pack_double(_state *mrb, _value o, _value str, _int sidx, unsigned int flags)
{
  int i;
  double d;
  uint8_t *buffer = (uint8_t *)&d;
  str = str_len_ensure(mrb, str, sidx + 8);
  d = _float(o);

  if (flags & PACK_FLAG_LITTLEENDIAN) {
#ifdef MRB_ENDIAN_BIG
    for (i = 0; i < 8; ++i) {
      RSTRING_PTR(str)[sidx + i] = buffer[8 - i - 1];
    }
#else
    memcpy(RSTRING_PTR(str) + sidx, buffer, 8);
#endif
  } else {
#ifdef MRB_ENDIAN_BIG
    memcpy(RSTRING_PTR(str) + sidx, buffer, 8);
#else
    for (i = 0; i < 8; ++i) {
      RSTRING_PTR(str)[sidx + i] = buffer[8 - i - 1];
    }
#endif
  }

  return 8;
}

static int
unpack_double(_state *mrb, const unsigned char * src, int srclen, _value ary, unsigned int flags)
{
  int i;
  double d;
  uint8_t *buffer = (uint8_t *)&d;

  if (flags & PACK_FLAG_LITTLEENDIAN) {
#ifdef MRB_ENDIAN_BIG
    for (i = 0; i < 8; ++i) {
      buffer[8 - i - 1] = src[i];
    }
#else
    memcpy(buffer, src, 8);
#endif
  } else {
#ifdef MRB_ENDIAN_BIG
    memcpy(buffer, src, 8);
#else
    for (i = 0; i < 8; ++i) {
      buffer[8 - i - 1] = src[i];
    }
#endif
  }
  _ary_push(mrb, ary, _float_value(mrb, d));

  return 8;
}

static int
pack_float(_state *mrb, _value o, _value str, _int sidx, unsigned int flags)
{
  int i;
  float f;
  uint8_t *buffer = (uint8_t *)&f;
  str = str_len_ensure(mrb, str, sidx + 4);
  f = (float)_float(o);

  if (flags & PACK_FLAG_LITTLEENDIAN) {
#ifdef MRB_ENDIAN_BIG
    for (i = 0; i < 4; ++i) {
      RSTRING_PTR(str)[sidx + i] = buffer[4 - i - 1];
    }
#else
    memcpy(RSTRING_PTR(str) + sidx, buffer, 4);
#endif
  } else {
#ifdef MRB_ENDIAN_BIG
    memcpy(RSTRING_PTR(str) + sidx, buffer, 4);
#else
    for (i = 0; i < 4; ++i) {
      RSTRING_PTR(str)[sidx + i] = buffer[4 - i - 1];
    }
#endif
  }

  return 4;
}

static int
unpack_float(_state *mrb, const unsigned char * src, int srclen, _value ary, unsigned int flags)
{
  int i;
  float f;
  uint8_t *buffer = (uint8_t *)&f;

  if (flags & PACK_FLAG_LITTLEENDIAN) {
#ifdef MRB_ENDIAN_BIG
    for (i = 0; i < 4; ++i) {
      buffer[4 - i - 1] = src[i];
    }
#else
    memcpy(buffer, src, 4);
#endif
  } else {
#ifdef MRB_ENDIAN_BIG
    memcpy(buffer, src, 4);
#else
    for (i = 0; i < 4; ++i) {
      buffer[4 - i - 1] = src[i];
    }
#endif
  }
  _ary_push(mrb, ary, _float_value(mrb, f));

  return 4;
}
#endif

static int
pack_utf8(_state *mrb, _value o, _value str, _int sidx, long count, unsigned int flags)
{
  char utf8[4];
  int len = 0;
  uint32_t c = 0;

#ifndef MRB_WITHOUT_FLOAT
  if (_float_p(o)) {
    goto range_error;
  }
#endif
  c = (uint32_t)_fixnum(o);

  /* Unicode character */
  /* from mruby-compiler gem */
  if (c < 0x80) {
    utf8[0] = (char)c;
    len = 1;
  }
  else if (c < 0x800) {
    utf8[0] = (char)(0xC0 | (c >> 6));
    utf8[1] = (char)(0x80 | (c & 0x3F));
    len = 2;
  }
  else if (c < 0x10000) {
    utf8[0] = (char)(0xE0 |  (c >> 12)        );
    utf8[1] = (char)(0x80 | ((c >>  6) & 0x3F));
    utf8[2] = (char)(0x80 | ( c        & 0x3F));
    len = 3;
  }
  else if (c < 0x200000) {
    utf8[0] = (char)(0xF0 |  (c >> 18)        );
    utf8[1] = (char)(0x80 | ((c >> 12) & 0x3F));
    utf8[2] = (char)(0x80 | ((c >>  6) & 0x3F));
    utf8[3] = (char)(0x80 | ( c        & 0x3F));
    len = 4;
  }
  else {
#ifndef MRB_WITHOUT_FLOAT
range_error:
#endif
    _raise(mrb, E_RANGE_ERROR, "pack(U): value out of range");
  }

  str = str_len_ensure(mrb, str, sidx + len);
  memcpy(RSTRING_PTR(str) + sidx, utf8, len);

  return len;
}

static const unsigned long utf8_limits[] = {
  0x0,        /* 1 */
  0x80,       /* 2 */
  0x800,      /* 3 */
  0x10000,    /* 4 */
  0x200000,   /* 5 */
  0x4000000,  /* 6 */
  0x80000000, /* 7 */
};

static unsigned long
utf8_to_uv(_state *mrb, const char *p, long *lenp)
{
  int c = *p++ & 0xff;
  unsigned long uv = c;
  long n = 1;

  if (!(uv & 0x80)) {
    *lenp = 1;
    return uv;
  }
  if (!(uv & 0x40)) {
    *lenp = 1;
    _raise(mrb, E_ARGUMENT_ERROR, "malformed UTF-8 character");
  }

  if      (!(uv & 0x20)) { n = 2; uv &= 0x1f; }
  else if (!(uv & 0x10)) { n = 3; uv &= 0x0f; }
  else if (!(uv & 0x08)) { n = 4; uv &= 0x07; }
  else if (!(uv & 0x04)) { n = 5; uv &= 0x03; }
  else if (!(uv & 0x02)) { n = 6; uv &= 0x01; }
  else {
    *lenp = 1;
    _raise(mrb, E_ARGUMENT_ERROR, "malformed UTF-8 character");
  }
  if (n > *lenp) {
    _raisef(mrb, E_ARGUMENT_ERROR, "malformed UTF-8 character (expected %S bytes, given %S bytes)",
               _fixnum_value(n), _fixnum_value(*lenp));
  }
  *lenp = n--;
  if (n != 0) {
    while (n--) {
      c = *p++ & 0xff;
      if ((c & 0xc0) != 0x80) {
        *lenp -= n + 1;
        _raise(mrb, E_ARGUMENT_ERROR, "malformed UTF-8 character");
      }
      else {
        c &= 0x3f;
        uv = uv << 6 | c;
      }
    }
  }
  n = *lenp - 1;
  if (uv < utf8_limits[n]) {
    _raise(mrb, E_ARGUMENT_ERROR, "redundant UTF-8 sequence");
  }
  return uv;
}

static int
unpack_utf8(_state *mrb, const unsigned char * src, int srclen, _value ary, unsigned int flags)
{
  unsigned long uv;
  long lenp = srclen;

  if (srclen == 0) {
    return 1;
  }
  uv = utf8_to_uv(mrb, (const char *)src, &lenp);
  _ary_push(mrb, ary, _fixnum_value((_int)uv));
  return (int)lenp;
}

static int
pack_a(_state *mrb, _value src, _value dst, _int didx, long count, unsigned int flags)
{
  _int copylen, slen, padlen;
  char *dptr, *dptr0, pad, *sptr;

  sptr = RSTRING_PTR(src);
  slen = RSTRING_LEN(src);

  if ((flags & PACK_FLAG_a) || (flags & PACK_FLAG_Z))
    pad = '\0';
  else
    pad = ' ';

  if (count == 0) {
    return 0;
  } else if (count == -1) {
    copylen = slen;
    padlen = (flags & PACK_FLAG_Z) ? 1 : 0;
  } else if (count < slen) {
    copylen = count;
    padlen = 0;
  } else {
    copylen = slen;
    padlen = count - slen;
  }

  dst = str_len_ensure(mrb, dst, didx + copylen + padlen);
  dptr0 = dptr = RSTRING_PTR(dst) + didx;
  memcpy(dptr, sptr, copylen);
  dptr += copylen;
  while (padlen-- > 0) {
    *dptr++ = pad;
  }

  return (int)(dptr - dptr0);
}

static int
unpack_a(_state *mrb, const void *src, int slen, _value ary, long count, unsigned int flags)
{
  _value dst;
  const char *cp, *sptr;
  int copylen;

  sptr = (const char *)src;
  if (count != -1 && count < slen)  {
    slen = count;
  }
  copylen = slen;

  if (slen >= 0 && flags & PACK_FLAG_Z) {  /* "Z" */
    if ((cp = (const char *)memchr(sptr, '\0', slen)) != NULL) {
      copylen = (int)(cp - sptr);
      if (count == -1) {
        slen = copylen + 1;
      }
    }
  }
  else if (!(flags & PACK_FLAG_a)) {  /* "A" */
    while (copylen > 0 && (sptr[copylen - 1] == '\0' || isspace(sptr[copylen - 1]))) {
      copylen--;
    }
  }

  if (copylen < 0) copylen = 0;
  dst = _str_new(mrb, sptr, (_int)copylen);
  _ary_push(mrb, ary, dst);
  return slen;
}


static int
pack_h(_state *mrb, _value src, _value dst, _int didx, long count, unsigned int flags)
{
  unsigned int a, ashift, b, bshift;
  long slen;
  char *dptr, *dptr0, *sptr;

  sptr = RSTRING_PTR(src);
  slen = (long)RSTRING_LEN(src);

  if (flags & PACK_FLAG_LSB) {
    ashift = 0;
    bshift = 4;
  } else {
    ashift = 4;
    bshift = 0;
  }

  if (count == -1) {
    count = slen;
  } else if (slen > count) {
    slen = count;
  }

  dst = str_len_ensure(mrb, dst, didx + count);
  dptr = RSTRING_PTR(dst) + didx;

  dptr0 = dptr;
  for (; count > 0; count -= 2) {
    a = b = 0;
    if (slen > 0) {
      a = hex2int(*sptr++);
      slen--;
    }
    if (slen > 0) {
      b = hex2int(*sptr++);
      slen--;
    }
    *dptr++ = (a << ashift) + (b << bshift);
  }

  return (int)(dptr - dptr0);
}

static int
unpack_h(_state *mrb, const void *src, int slen, _value ary, int count, unsigned int flags)
{
  _value dst;
  int a, ashift, b, bshift;
  const char *sptr, *sptr0;
  char *dptr, *dptr0;
  const char hexadecimal[] = "0123456789abcdef";

  if (flags & PACK_FLAG_LSB) {
    ashift = 0;
    bshift = 4;
  } else {
    ashift = 4;
    bshift = 0;
  }

  sptr = (const char *)src;

  if (count == -1)
    count = slen * 2;

  dst = _str_new(mrb, NULL, count);
  dptr = RSTRING_PTR(dst);

  sptr0 = sptr;
  dptr0 = dptr;
  while (slen > 0 && count > 0) {
    a = (*sptr >> ashift) & 0x0f;
    b = (*sptr >> bshift) & 0x0f;
    sptr++;
    slen--;

    *dptr++ = hexadecimal[a];
    count--;

    if (count > 0) {
      *dptr++ = hexadecimal[b];
      count--;
    }
  }

  dst = _str_resize(mrb, dst, dptr - dptr0);
  _ary_push(mrb, ary, dst);
  return (int)(sptr - sptr0);
}


static int
pack_m(_state *mrb, _value src, _value dst, _int didx, long count, unsigned int flags)
{
  _int dstlen;
  unsigned long l;
  _int column, srclen;
  char *srcptr, *dstptr, *dstptr0;

  srcptr = RSTRING_PTR(src);
  srclen = RSTRING_LEN(src);

  if (srclen == 0)  /* easy case */
    return 0;

  if (count != 0 && count < 3) {  /* -1, 1 or 2 */
    count = 45;
  } else if (count >= 3) {
    count -= count % 3;
  }

  dstlen = (srclen+2) / 3 * 4;
  if (count > 0) {
    dstlen += (srclen / count) + ((srclen % count) == 0 ? 0 : 1);
  }
  dst = str_len_ensure(mrb, dst, didx + dstlen);
  dstptr = RSTRING_PTR(dst) + didx;

  dstptr0 = dstptr;
  for (column = 3; srclen >= 3; srclen -= 3, column += 3) {
    l = (unsigned char)*srcptr++ << 16;
    l += (unsigned char)*srcptr++ << 8;
    l += (unsigned char)*srcptr++;

    *dstptr++ = base64chars[(l >> 18) & 0x3f];
    *dstptr++ = base64chars[(l >> 12) & 0x3f];
    *dstptr++ = base64chars[(l >>  6) & 0x3f];
    *dstptr++ = base64chars[ l        & 0x3f];

    if (column == count) {
      *dstptr++ = '\n';
      column = 0;
    }
  }
  if (srclen == 1) {
    l = (unsigned char)*srcptr++ << 16;
    *dstptr++ = base64chars[(l >> 18) & 0x3f];
    *dstptr++ = base64chars[(l >> 12) & 0x3f];
    *dstptr++ = '=';
    *dstptr++ = '=';
    column += 3;
  } else if (srclen == 2) {
    l = (unsigned char)*srcptr++ << 16;
    l += (unsigned char)*srcptr++ << 8;
    *dstptr++ = base64chars[(l >> 18) & 0x3f];
    *dstptr++ = base64chars[(l >> 12) & 0x3f];
    *dstptr++ = base64chars[(l >>  6) & 0x3f];
    *dstptr++ = '=';
    column += 3;
  }
  if (column > 0 && count > 0) {
    *dstptr++ = '\n';
  }

  return (int)(dstptr - dstptr0);
}

static int
unpack_m(_state *mrb, const void *src, int slen, _value ary, unsigned int flags)
{
  _value dst;
  int dlen;
  unsigned long l;
  int i, padding;
  unsigned char c, ch[4];
  const char *sptr, *sptr0;
  char *dptr, *dptr0;

  sptr0 = sptr = (const char *)src;

  dlen = slen / 4 * 3;  /* an estimated value - may be shorter */
  dst = _str_new(mrb, NULL, dlen);
  dptr0 = dptr = RSTRING_PTR(dst);

  padding = 0;
  while (slen >= 4) {
    for (i = 0; i < 4; i++) {
      do {
        if (slen-- == 0)
          goto done;
        c = *sptr++;
	if (c >= sizeof(base64_dec_tab))
	  continue;
	ch[i] = base64_dec_tab[c];
	if (ch[i] == PACK_BASE64_PADDING) {
	  ch[i] = 0;
	  padding++;
	}
      } while (c >= sizeof(base64_dec_tab) || ch[i] == PACK_BASE64_IGNORE);
    }

    l = (ch[0] << 18) + (ch[1] << 12) + (ch[2] << 6) + ch[3];

    if (padding == 0) {
      *dptr++ = (l >> 16) & 0xff;
      *dptr++ = (l >> 8) & 0xff;
      *dptr++ = l & 0xff;
    } else if (padding == 1) {
      *dptr++ = (l >> 16) & 0xff;
      *dptr++ = (l >> 8) & 0xff;
      break;
    } else {
      *dptr++ = (l >> 16) & 0xff;
      break;
    }
  }

done:
  dst = _str_resize(mrb, dst, dptr - dptr0);
  _ary_push(mrb, ary, dst);
  return (int)(sptr - sptr0);
}

static int
pack_x(_state *mrb, _value src, _value dst, _int didx, long count, unsigned int flags)
{
  long i;

  if (count < 0) return 0;
  dst = str_len_ensure(mrb, dst, didx + count);
  for (i = 0; i < count; i++) {
    RSTRING_PTR(dst)[didx + i] = '\0';
  }
  return count;
}
static int
unpack_x(_state *mrb, const void *src, int slen, _value ary, int count, unsigned int flags)
{
  if (count < 0) return slen;
  if (slen < count) {
    _raise(mrb, E_ARGUMENT_ERROR, "x outside of string");
  }
  return count;
}

static void
prepare_tmpl(_state *mrb, struct tmpl *tmpl)
{
  _get_args(mrb, "S", &tmpl->str);
  tmpl->idx = 0;
}

static int
has_tmpl(const struct tmpl *tmpl)
{
  return (tmpl->idx < RSTRING_LEN(tmpl->str));
}

static void
read_tmpl(_state *mrb, struct tmpl *tmpl, int *dirp, int *typep, int *sizep, int *countp, unsigned int *flagsp)
{
  _int t, tlen;
  int ch, dir, type, size = 0;
  int count = 1;
  unsigned int flags = 0;
  const char *tptr;

  tptr = RSTRING_PTR(tmpl->str);
  tlen = RSTRING_LEN(tmpl->str);

  t = tptr[tmpl->idx++];
alias:
  switch (t) {
  case 'A':
    dir = PACK_DIR_STR;
    type = PACK_TYPE_STRING;
    flags |= PACK_FLAG_WIDTH | PACK_FLAG_COUNT2;
    break;
  case 'a':
    dir = PACK_DIR_STR;
    type = PACK_TYPE_STRING;
    flags |= PACK_FLAG_WIDTH | PACK_FLAG_COUNT2 | PACK_FLAG_a;
    break;
  case 'C':
    dir = PACK_DIR_CHAR;
    type = PACK_TYPE_INTEGER;
    size = 1;
    break;
  case 'c':
    dir = PACK_DIR_CHAR;
    type = PACK_TYPE_INTEGER;
    size = 1;
    flags |= PACK_FLAG_SIGNED;
    break;
  case 'D': case 'd':
    dir = PACK_DIR_DOUBLE;
    type = PACK_TYPE_FLOAT;
    size = 8;
    flags |= PACK_FLAG_SIGNED;
    break;
  case 'F': case 'f':
    dir = PACK_DIR_FLOAT;
    type = PACK_TYPE_FLOAT;
    size = 4;
    flags |= PACK_FLAG_SIGNED;
    break;
  case 'E':
    dir = PACK_DIR_DOUBLE;
    type = PACK_TYPE_FLOAT;
    size = 8;
    flags |= PACK_FLAG_SIGNED | PACK_FLAG_LT;
    break;
  case 'e':
    dir = PACK_DIR_FLOAT;
    type = PACK_TYPE_FLOAT;
    size = 4;
    flags |= PACK_FLAG_SIGNED | PACK_FLAG_LT;
    break;
  case 'G':
    dir = PACK_DIR_DOUBLE;
    type = PACK_TYPE_FLOAT;
    size = 8;
    flags |= PACK_FLAG_SIGNED | PACK_FLAG_GT;
    break;
  case 'g':
    dir = PACK_DIR_FLOAT;
    type = PACK_TYPE_FLOAT;
    size = 4;
    flags |= PACK_FLAG_SIGNED | PACK_FLAG_GT;
    break;
  case 'H':
    dir = PACK_DIR_HEX;
    type = PACK_TYPE_STRING;
    flags |= PACK_FLAG_COUNT2;
    break;
  case 'h':
    dir = PACK_DIR_HEX;
    type = PACK_TYPE_STRING;
    flags |= PACK_FLAG_COUNT2 | PACK_FLAG_LSB;
    break;
  case 'I':
    switch (sizeof(int)) {
      case 2: t = 'S'; goto alias;
      case 4: t = 'L'; goto alias;
      case 8: t = 'Q'; goto alias;
      default:
        _raisef(mrb, E_RUNTIME_ERROR, "mruby-pack does not support sizeof(int) == %S", _fixnum_value(sizeof(int)));
    }
    break;
  case 'i':
    switch (sizeof(int)) {
      case 2: t = 's'; goto alias;
      case 4: t = 'l'; goto alias;
      case 8: t = 'q'; goto alias;
      default:
        _raisef(mrb, E_RUNTIME_ERROR, "mruby-pack does not support sizeof(int) == %S", _fixnum_value(sizeof(int)));
    }
    break;
  case 'L':
    dir = PACK_DIR_LONG;
    type = PACK_TYPE_INTEGER;
    size = 4;
    break;
  case 'l':
    dir = PACK_DIR_LONG;
    type = PACK_TYPE_INTEGER;
    size = 4;
    flags |= PACK_FLAG_SIGNED;
    break;
  case 'm':
    dir = PACK_DIR_BASE64;
    type = PACK_TYPE_STRING;
    flags |= PACK_FLAG_WIDTH;
    break;
  case 'N':  /* = "L>" */
    dir = PACK_DIR_LONG;
    type = PACK_TYPE_INTEGER;
    size = 4;
    flags |= PACK_FLAG_GT;
    break;
  case 'n':  /* = "S>" */
    dir = PACK_DIR_SHORT;
    type = PACK_TYPE_INTEGER;
    size = 2;
    flags |= PACK_FLAG_GT;
    break;
  case 'Q':
    dir = PACK_DIR_QUAD;
    type = PACK_TYPE_INTEGER;
    size = 8;
    break;
  case 'q':
    dir = PACK_DIR_QUAD;
    type = PACK_TYPE_INTEGER;
    size = 8;
    flags |= PACK_FLAG_SIGNED;
    break;
  case 'S':
    dir = PACK_DIR_SHORT;
    type = PACK_TYPE_INTEGER;
    size = 2;
    break;
  case 's':
    dir = PACK_DIR_SHORT;
    type = PACK_TYPE_INTEGER;
    size = 2;
    flags |= PACK_FLAG_SIGNED;
    break;
  case 'U':
    dir = PACK_DIR_UTF8;
    type = PACK_TYPE_INTEGER;
    break;
  case 'V':  /* = "L<" */
    dir = PACK_DIR_LONG;
    type = PACK_TYPE_INTEGER;
    size = 4;
    flags |= PACK_FLAG_LT;
    break;
  case 'v':  /* = "S<" */
    dir = PACK_DIR_SHORT;
    type = PACK_TYPE_INTEGER;
    size = 2;
    flags |= PACK_FLAG_LT;
    break;
  case 'x':
    dir = PACK_DIR_NUL;
    type = PACK_TYPE_NONE;
    break;
  case 'Z':
    dir = PACK_DIR_STR;
    type = PACK_TYPE_STRING;
    flags |= PACK_FLAG_WIDTH | PACK_FLAG_COUNT2 | PACK_FLAG_Z;
    break;
  default:
    dir = PACK_DIR_INVALID;
    type = PACK_TYPE_NONE;
    break;
  }

  /* read suffix [0-9*_!<>] */
  while (tmpl->idx < tlen) {
    ch = tptr[tmpl->idx++];
    if (isdigit(ch)) {
      count = ch - '0';
      while (tmpl->idx < tlen && isdigit(tptr[tmpl->idx])) {
        count = count * 10 + (tptr[tmpl->idx++] - '0');
        if (count < 0) {
          _raise(mrb, E_RUNTIME_ERROR, "too big template length");
        }
      }
      continue;  /* special case */
    } else if (ch == '*')  {
      count = -1;
    } else if (ch == '_' || ch == '!' || ch == '<' || ch == '>') {
      if (strchr("sSiIlLqQ", (int)t) == NULL) {
        char ch_str = (char)ch;
        _raisef(mrb, E_ARGUMENT_ERROR, "'%S' allowed only after types sSiIlLqQ", _str_new(mrb, &ch_str, 1));
      }
      if (ch == '_' || ch == '!') {
        flags |= PACK_FLAG_s;
      } else if (ch == '<') {
        flags |= PACK_FLAG_LT;
      } else if (ch == '>') {
        flags |= PACK_FLAG_GT;
      }
    } else {
      tmpl->idx--;
      break;
    }
  }

  if ((flags & PACK_FLAG_LT) || (!(flags & PACK_FLAG_GT) && littleendian)) {
    flags |= PACK_FLAG_LITTLEENDIAN;
  }

  *dirp = dir;
  *typep = type;
  *sizep = size;
  *countp = count;
  *flagsp = flags;
}

static _value
_pack_pack(_state *mrb, _value ary)
{
  _value o, result;
  _int aidx;
  struct tmpl tmpl;
  int count;
  unsigned int flags;
  int dir, ridx, size, type;

  prepare_tmpl(mrb, &tmpl);

  result = _str_new(mrb, NULL, 128);  /* allocate initial buffer */
  aidx = 0;
  ridx = 0;
  while (has_tmpl(&tmpl)) {
    read_tmpl(mrb, &tmpl, &dir, &type, &size, &count, &flags);

    if (dir == PACK_DIR_INVALID)
      continue;
    else if (dir == PACK_DIR_NUL) {
        ridx += pack_x(mrb, _nil_value(), result, ridx, count, flags);
        continue;
    }

    for (; aidx < RARRAY_LEN(ary); aidx++) {
      if (count == 0 && !(flags & PACK_FLAG_WIDTH))
        break;

      o = _ary_ref(mrb, ary, aidx);
      if (type == PACK_TYPE_INTEGER) {
        o = _to_int(mrb, o);
#ifndef MRB_WITHOUT_FLOAT
      } else if (type == PACK_TYPE_FLOAT) {
        if (!_float_p(o)) {
          _float f = _to_flo(mrb, o);
          o = _float_value(mrb, f);
        }
#endif
      } else if (type == PACK_TYPE_STRING) {
        if (!_string_p(o)) {
          _raisef(mrb, E_TYPE_ERROR, "can't convert %S into String", _class_path(mrb, _obj_class(mrb, o)));
        }
      }

      switch (dir) {
      case PACK_DIR_CHAR:
        ridx += pack_c(mrb, o, result, ridx, flags);
        break;
      case PACK_DIR_SHORT:
        ridx += pack_s(mrb, o, result, ridx, flags);
        break;
      case PACK_DIR_LONG:
        ridx += pack_l(mrb, o, result, ridx, flags);
        break;
      case PACK_DIR_QUAD:
        ridx += pack_q(mrb, o, result, ridx, flags);
        break;
      case PACK_DIR_BASE64:
        ridx += pack_m(mrb, o, result, ridx, count, flags);
        break;
      case PACK_DIR_HEX:
        ridx += pack_h(mrb, o, result, ridx, count, flags);
        break;
      case PACK_DIR_STR:
        ridx += pack_a(mrb, o, result, ridx, count, flags);
        break;
#ifndef MRB_WITHOUT_FLOAT
      case PACK_DIR_DOUBLE:
        ridx += pack_double(mrb, o, result, ridx, flags);
        break;
      case PACK_DIR_FLOAT:
        ridx += pack_float(mrb, o, result, ridx, flags);
        break;
#endif
      case PACK_DIR_UTF8:
        ridx += pack_utf8(mrb, o, result, ridx, count, flags);
        break;
      default:
        break;
      }
      if (dir == PACK_DIR_STR) { /* always consumes 1 entry */
        aidx++;
        break;
      }
      if (count > 0) {
        count--;
      }
    }
    if (ridx < 0) {
      _raise(mrb, E_RANGE_ERROR, "negative (or overflowed) template size");
    }
  }

  _str_resize(mrb, result, ridx);
  return result;
}

static _value
pack_unpack(_state *mrb, _value str, int single)
{
  _value result;
  struct tmpl tmpl;
  int count;
  unsigned int flags;
  int dir, size, type;
  int srcidx, srclen;
  const unsigned char *sptr;

  prepare_tmpl(mrb, &tmpl);

  srcidx = 0;
  srclen = (int)RSTRING_LEN(str);

  result = _ary_new(mrb);
  while (has_tmpl(&tmpl)) {
    read_tmpl(mrb, &tmpl, &dir, &type, &size, &count, &flags);

    if (dir == PACK_DIR_INVALID)
      continue;
    else if (dir == PACK_DIR_NUL) {
      srcidx += unpack_x(mrb, sptr, srclen - srcidx, result, count, flags);
      continue;
    }

    if (flags & PACK_FLAG_COUNT2) {
      sptr = (const unsigned char *)RSTRING_PTR(str) + srcidx;
      switch (dir) {
      case PACK_DIR_HEX:
        srcidx += unpack_h(mrb, sptr, srclen - srcidx, result, count, flags);
        break;
      case PACK_DIR_STR:
        srcidx += unpack_a(mrb, sptr, srclen - srcidx, result, count, flags);
        break;
      }
      continue;
    }

    while (count != 0) {
      if (srclen - srcidx < size) {
        while (count-- > 0) {
          _ary_push(mrb, result, _nil_value());
        }
        break;
      }

      sptr = (const unsigned char*)RSTRING_PTR(str) + srcidx;
      switch (dir) {
      case PACK_DIR_CHAR:
        srcidx += unpack_c(mrb, sptr, srclen - srcidx, result, flags);
        break;
      case PACK_DIR_SHORT:
        srcidx += unpack_s(mrb, sptr, srclen - srcidx, result, flags);
        break;
      case PACK_DIR_LONG:
        srcidx += unpack_l(mrb, sptr, srclen - srcidx, result, flags);
        break;
      case PACK_DIR_QUAD:
        srcidx += unpack_q(mrb, sptr, srclen - srcidx, result, flags);
        break;
      case PACK_DIR_BASE64:
        srcidx += unpack_m(mrb, sptr, srclen - srcidx, result, flags);
        break;
#ifndef MRB_WITHOUT_FLOAT
      case PACK_DIR_FLOAT:
        srcidx += unpack_float(mrb, sptr, srclen - srcidx, result, flags);
        break;
      case PACK_DIR_DOUBLE:
        srcidx += unpack_double(mrb, sptr, srclen - srcidx, result, flags);
        break;
#endif
      case PACK_DIR_UTF8:
        srcidx += unpack_utf8(mrb, sptr, srclen - srcidx, result, flags);
        break;
      default:
        _raise(mrb, E_RUNTIME_ERROR, "mruby-pack's bug");
      }
      if (count > 0) {
        count--;
      }
    }
    if (single) break;
  }

  if (single) return RARRAY_PTR(result)[0];
  return result;
}

static _value
_pack_unpack(_state *mrb, _value str)
{
  return pack_unpack(mrb, str, 0);
}

static _value
_pack_unpack1(_state *mrb, _value str)
{
  return pack_unpack(mrb, str, 1);
}

void
_mruby_pack_gem_init(_state *mrb)
{
  littleendian = check_little_endian();
  make_base64_dec_tab();

  _define_method(mrb, mrb->array_class, "pack", _pack_pack, MRB_ARGS_REQ(1));
  _define_method(mrb, mrb->string_class, "unpack", _pack_unpack, MRB_ARGS_REQ(1));
  _define_method(mrb, mrb->string_class, "unpack1", _pack_unpack1, MRB_ARGS_REQ(1));
}

void
_mruby_pack_gem_final(_state *mrb)
{
}
