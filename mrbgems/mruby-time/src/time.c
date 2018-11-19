/*
** time.c - Time class
**
** See Copyright Notice in mruby.h
*/

#include <math.h>
#include <time.h>
#include <mruby.h>
#include <mruby/class.h>
#include <mruby/data.h>

#ifndef MRB_DISABLE_STDIO
#include <stdio.h>
#else
#include <string.h>
#endif

#define NDIV(x,y) (-(-((x)+1)/(y))-1)

#if defined(_MSC_VER) && _MSC_VER < 1800
double round(double x) {
  return floor(x + 0.5);
}
#endif

#if !defined(__MINGW64__) && defined(_WIN32)
# define llround(x) round(x)
#endif

#if defined(__MINGW64__) || defined(__MINGW32__)
# include <sys/time.h>
#endif

/** Time class configuration */

/* gettimeofday(2) */
/* C99 does not have gettimeofday that is required to retrieve microseconds */
/* uncomment following macro on platforms without gettimeofday(2) */
/* #define NO_GETTIMEOFDAY */

/* gmtime(3) */
/* C99 does not have reentrant gmtime_r() so it might cause troubles under */
/* multi-threading environment.  undef following macro on platforms that */
/* does not have gmtime_r() and localtime_r(). */
/* #define NO_GMTIME_R */

#ifdef _WIN32
#if _MSC_VER
/* Win32 platform do not provide gmtime_r/localtime_r; emulate them using gmtime_s/localtime_s */
#define gmtime_r(tp, tm)    ((gmtime_s((tm), (tp)) == 0) ? (tm) : NULL)
#define localtime_r(tp, tm)    ((localtime_s((tm), (tp)) == 0) ? (tm) : NULL)
#else
#define NO_GMTIME_R
#endif
#endif

/* asctime(3) */
/* mruby usually use its own implementation of struct tm to string conversion */
/* except when DISABLE_STDIO is set. In that case, it uses asctime() or asctime_r(). */
/* By default mruby tries to use asctime_r() which is reentrant. */
/* Undef following macro on platforms that does not have asctime_r(). */
/* #define NO_ASCTIME_R */

/* timegm(3) */
/* mktime() creates tm structure for localtime; timegm() is for UTC time */
/* define following macro to use probably faster timegm() on the platform */
/* #define USE_SYSTEM_TIMEGM */

/* time_t */
/* If your platform supports time_t as uint (e.g. uint32_t, uint64_t), */
/* uncomment following macro. */
/* #define MRB_TIME_T_UINT */

/** end of Time class configuration */

#ifndef NO_GETTIMEOFDAY
# ifdef _WIN32
#  define WIN32_LEAN_AND_MEAN  /* don't include winsock.h */
#  include <windows.h>
#  define gettimeofday my_gettimeofday

#  ifdef _MSC_VER
#    define UI64(x) x##ui64
#  else
#    define UI64(x) x##ull
#  endif

typedef long suseconds_t;

# if (!defined __MINGW64__) && (!defined __MINGW32__)
struct timeval {
  time_t tv_sec;
  suseconds_t tv_usec;
};
# endif

static int
gettimeofday(struct timeval *tv, void *tz)
{
  if (tz) {
    _assert(0);  /* timezone is not supported */
  }
  if (tv) {
    union {
      FILETIME ft;
      unsigned __int64 u64;
    } t;
    GetSystemTimeAsFileTime(&t.ft);   /* 100 ns intervals since Windows epoch */
    t.u64 -= UI64(116444736000000000);  /* Unix epoch bias */
    t.u64 /= 10;                      /* to microseconds */
    tv->tv_sec = (time_t)(t.u64 / (1000 * 1000));
    tv->tv_usec = t.u64 % (1000 * 1000);
  }
  return 0;
}
# else
#  include <sys/time.h>
# endif
#endif
#ifdef NO_GMTIME_R
#define gmtime_r(t,r) gmtime(t)
#define localtime_r(t,r) localtime(t)
#endif

#ifndef USE_SYSTEM_TIMEGM
#define timegm my_timgm

static unsigned int
is_leapyear(unsigned int y)
{
  return (y % 4) == 0 && ((y % 100) != 0 || (y % 400) == 0);
}

static time_t
timegm(struct tm *tm)
{
  static const unsigned int ndays[2][12] = {
    {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31},
    {31, 29, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31}
  };
  time_t r = 0;
  int i;
  unsigned int *nday = (unsigned int*) ndays[is_leapyear(tm->tm_year+1900)];

  static const int epoch_year = 70;
  if(tm->tm_year >= epoch_year) {
    for (i = epoch_year; i < tm->tm_year; ++i)
      r += is_leapyear(i+1900) ? 366*24*60*60 : 365*24*60*60;
  } else {
    for (i = tm->tm_year; i < epoch_year; ++i)
      r -= is_leapyear(i+1900) ? 366*24*60*60 : 365*24*60*60;
  }
  for (i = 0; i < tm->tm_mon; ++i)
    r += nday[i] * 24 * 60 * 60;
  r += (tm->tm_mday - 1) * 24 * 60 * 60;
  r += tm->tm_hour * 60 * 60;
  r += tm->tm_min * 60;
  r += tm->tm_sec;
  return r;
}
#endif

/* Since we are limited to using ISO C99, this implementation is based
* on time_t. That means the resolution of time is only precise to the
* second level. Also, there are only 2 timezones, namely UTC and LOCAL.
*/

enum mrb_timezone {
  MRB_TIMEZONE_NONE   = 0,
  MRB_TIMEZONE_UTC    = 1,
  MRB_TIMEZONE_LOCAL  = 2,
  MRB_TIMEZONE_LAST   = 3
};

typedef struct _timezone_name {
  const char name[8];
  size_t len;
} _timezone_name;

static const _timezone_name timezone_names[] = {
  { "none", sizeof("none") - 1 },
  { "UTC", sizeof("UTC") - 1 },
  { "LOCAL", sizeof("LOCAL") - 1 },
};

#ifndef MRB_DISABLE_STDIO
static const char mon_names[12][4] = {
  "Jan", "Feb", "Mar", "Apr", "May", "Jun", "Jul", "Aug", "Sep", "Oct", "Nov", "Dec",
};

static const char wday_names[7][4] = {
  "Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat",
};
#endif

struct _time {
  time_t              sec;
  time_t              usec;
  enum mrb_timezone   timezone;
  struct tm           datetime;
};

static const struct _data_type _time_type = { "Time", _free };

/** Updates the datetime of a _time based on it's timezone and
seconds setting. Returns self on success, NULL of failure. */
static struct _time*
time_update_datetime(_state *mrb, struct _time *self)
{
  struct tm *aid;

  if (self->timezone == MRB_TIMEZONE_UTC) {
    aid = gmtime_r(&self->sec, &self->datetime);
  }
  else {
    aid = localtime_r(&self->sec, &self->datetime);
  }
  if (!aid) {
    _raisef(mrb, E_ARGUMENT_ERROR, "%S out of Time range", _float_value(mrb, (_float)self->sec));
    /* not reached */
    return NULL;
  }
#ifdef NO_GMTIME_R
  self->datetime = *aid; /* copy data */
#endif

  return self;
}

static _value
_time_wrap(_state *mrb, struct RClass *tc, struct _time *tm)
{
  return _obj_value(Data_Wrap_Struct(mrb, tc, &_time_type, tm));
}

void _check_num_exact(_state *mrb, _float num);

/* Allocates a _time object and initializes it. */
static struct _time*
time_alloc(_state *mrb, double sec, double usec, enum mrb_timezone timezone)
{
  struct _time *tm;
  time_t tsec = 0;

  _check_num_exact(mrb, (_float)sec);
  _check_num_exact(mrb, (_float)usec);
#ifndef MRB_TIME_T_UINT
  if (sizeof(time_t) == 4 && (sec > (double)INT32_MAX || (double)INT32_MIN > sec)) {
    goto out_of_range;
  }
  if (sizeof(time_t) == 8 && (sec > (double)INT64_MAX || (double)INT64_MIN > sec)) {
    goto out_of_range;
  }
#else
  if (sizeof(time_t) == 4 && (sec > (double)UINT32_MAX || (double)0 > sec)) {
    goto out_of_range;
  }
  if (sizeof(time_t) == 8 && (sec > (double)UINT64_MAX || (double)0 > sec)) {
    goto out_of_range;
  }
#endif
  tsec  = (time_t)sec;
  if ((sec > 0 && tsec < 0) || (sec < 0 && (double)tsec > sec)) {
  out_of_range:
    _raisef(mrb, E_ARGUMENT_ERROR, "%S out of Time range", _float_value(mrb, sec));
  }
  tm = (struct _time *)_malloc(mrb, sizeof(struct _time));
  tm->sec  = tsec;
  tm->usec = (time_t)llround((sec - tm->sec) * 1.0e6 + usec);
  if (tm->usec < 0) {
    long sec2 = (long)NDIV(usec,1000000); /* negative div */
    tm->usec -= sec2 * 1000000;
    tm->sec += sec2;
  }
  else if (tm->usec >= 1000000) {
    long sec2 = (long)(usec / 1000000);
    tm->usec -= sec2 * 1000000;
    tm->sec += sec2;
  }
  tm->timezone = timezone;
  time_update_datetime(mrb, tm);

  return tm;
}

static _value
_time_make(_state *mrb, struct RClass *c, double sec, double usec, enum mrb_timezone timezone)
{
  return _time_wrap(mrb, c, time_alloc(mrb, sec, usec, timezone));
}

static struct _time*
current__time(_state *mrb)
{
  struct _time *tm;

  tm = (struct _time *)_malloc(mrb, sizeof(*tm));
#if defined(TIME_UTC)
  {
    struct timespec ts;
    if (timespec_get(&ts, TIME_UTC) == 0) {
      _free(mrb, tm);
      _raise(mrb, E_RUNTIME_ERROR, "timespec_get() failed for unknown reasons");
    }
    tm->sec = ts.tv_sec;
    tm->usec = ts.tv_nsec / 1000;
  }
#elif defined(NO_GETTIMEOFDAY)
  {
    static time_t last_sec = 0, last_usec = 0;

    tm->sec  = time(NULL);
    if (tm->sec != last_sec) {
      last_sec = tm->sec;
      last_usec = 0;
    }
    else {
      /* add 1 usec to differentiate two times */
      last_usec += 1;
    }
    tm->usec = last_usec;
  }
#else
  {
    struct timeval tv;

    gettimeofday(&tv, NULL);
    tm->sec = tv.tv_sec;
    tm->usec = tv.tv_usec;
  }
#endif
  tm->timezone = MRB_TIMEZONE_LOCAL;
  time_update_datetime(mrb, tm);

  return tm;
}

/* Allocates a new Time object with given millis value. */
static _value
_time_now(_state *mrb, _value self)
{
  return _time_wrap(mrb, _class_ptr(self), current__time(mrb));
}

/* 15.2.19.6.1 */
/* Creates an instance of time at the given time in seconds, etc. */
static _value
_time_at(_state *mrb, _value self)
{
  _float f, f2 = 0;

  _get_args(mrb, "f|f", &f, &f2);
  return _time_make(mrb, _class_ptr(self), f, f2, MRB_TIMEZONE_LOCAL);
}

static struct _time*
time_mktime(_state *mrb, _int ayear, _int amonth, _int aday,
  _int ahour, _int amin, _int asec, _int ausec,
  enum mrb_timezone timezone)
{
  time_t nowsecs;
  struct tm nowtime = { 0 };

  nowtime.tm_year  = (int)ayear  - 1900;
  nowtime.tm_mon   = (int)amonth - 1;
  nowtime.tm_mday  = (int)aday;
  nowtime.tm_hour  = (int)ahour;
  nowtime.tm_min   = (int)amin;
  nowtime.tm_sec   = (int)asec;
  nowtime.tm_isdst = -1;

  if (nowtime.tm_mon  < 0 || nowtime.tm_mon  > 11
      || nowtime.tm_mday < 1 || nowtime.tm_mday > 31
      || nowtime.tm_hour < 0 || nowtime.tm_hour > 24
      || (nowtime.tm_hour == 24 && (nowtime.tm_min > 0 || nowtime.tm_sec > 0))
      || nowtime.tm_min  < 0 || nowtime.tm_min  > 59
      || nowtime.tm_sec  < 0 || nowtime.tm_sec  > 60)
    _raise(mrb, E_RUNTIME_ERROR, "argument out of range");

  if (timezone == MRB_TIMEZONE_UTC) {
    nowsecs = timegm(&nowtime);
  }
  else {
    nowsecs = mktime(&nowtime);
  }
  if (nowsecs == (time_t)-1) {
    _raise(mrb, E_ARGUMENT_ERROR, "Not a valid time.");
  }

  return time_alloc(mrb, (double)nowsecs, (double)ausec, timezone);
}

/* 15.2.19.6.2 */
/* Creates an instance of time at the given time in UTC. */
static _value
_time_gm(_state *mrb, _value self)
{
  _int ayear = 0, amonth = 1, aday = 1, ahour = 0, amin = 0, asec = 0, ausec = 0;

  _get_args(mrb, "i|iiiiii",
                &ayear, &amonth, &aday, &ahour, &amin, &asec, &ausec);
  return _time_wrap(mrb, _class_ptr(self),
          time_mktime(mrb, ayear, amonth, aday, ahour, amin, asec, ausec, MRB_TIMEZONE_UTC));
}


/* 15.2.19.6.3 */
/* Creates an instance of time at the given time in local time zone. */
static _value
_time_local(_state *mrb, _value self)
{
  _int ayear = 0, amonth = 1, aday = 1, ahour = 0, amin = 0, asec = 0, ausec = 0;

  _get_args(mrb, "i|iiiiii",
                &ayear, &amonth, &aday, &ahour, &amin, &asec, &ausec);
  return _time_wrap(mrb, _class_ptr(self),
          time_mktime(mrb, ayear, amonth, aday, ahour, amin, asec, ausec, MRB_TIMEZONE_LOCAL));
}

static struct _time*
time_get_ptr(_state *mrb, _value time)
{
  struct _time *tm;

  tm = DATA_GET_PTR(mrb, time, &_time_type, struct _time);
  if (!tm) {
    _raise(mrb, E_ARGUMENT_ERROR, "uninitialized time");
  }
  return tm;
}

static _value
_time_eq(_state *mrb, _value self)
{
  _value other;
  struct _time *tm1, *tm2;
  _bool eq_p;

  _get_args(mrb, "o", &other);
  tm1 = DATA_GET_PTR(mrb, self, &_time_type, struct _time);
  tm2 = DATA_CHECK_GET_PTR(mrb, other, &_time_type, struct _time);
  eq_p = tm1 && tm2 && tm1->sec == tm2->sec && tm1->usec == tm2->usec;

  return _bool_value(eq_p);
}

static _value
_time_cmp(_state *mrb, _value self)
{
  _value other;
  struct _time *tm1, *tm2;

  _get_args(mrb, "o", &other);
  tm1 = DATA_GET_PTR(mrb, self, &_time_type, struct _time);
  tm2 = DATA_CHECK_GET_PTR(mrb, other, &_time_type, struct _time);
  if (!tm1 || !tm2) return _nil_value();
  if (tm1->sec > tm2->sec) {
    return _fixnum_value(1);
  }
  else if (tm1->sec < tm2->sec) {
    return _fixnum_value(-1);
  }
  /* tm1->sec == tm2->sec */
  if (tm1->usec > tm2->usec) {
    return _fixnum_value(1);
  }
  else if (tm1->usec < tm2->usec) {
    return _fixnum_value(-1);
  }
  return _fixnum_value(0);
}

static _value
_time_plus(_state *mrb, _value self)
{
  _float f;
  struct _time *tm;

  _get_args(mrb, "f", &f);
  tm = time_get_ptr(mrb, self);
  return _time_make(mrb, _obj_class(mrb, self), (double)tm->sec+f, (double)tm->usec, tm->timezone);
}

static _value
_time_minus(_state *mrb, _value self)
{
  _float f;
  _value other;
  struct _time *tm, *tm2;

  _get_args(mrb, "o", &other);
  tm = time_get_ptr(mrb, self);
  tm2 = DATA_CHECK_GET_PTR(mrb, other, &_time_type, struct _time);
  if (tm2) {
    f = (_float)(tm->sec - tm2->sec)
      + (_float)(tm->usec - tm2->usec) / 1.0e6;
    return _float_value(mrb, f);
  }
  else {
    _get_args(mrb, "f", &f);
    return _time_make(mrb, _obj_class(mrb, self), (double)tm->sec-f, (double)tm->usec, tm->timezone);
  }
}

/* 15.2.19.7.30 */
/* Returns week day number of time. */
static _value
_time_wday(_state *mrb, _value self)
{
  struct _time *tm;

  tm = time_get_ptr(mrb, self);
  return _fixnum_value(tm->datetime.tm_wday);
}

/* 15.2.19.7.31 */
/* Returns year day number of time. */
static _value
_time_yday(_state *mrb, _value self)
{
  struct _time *tm;

  tm = time_get_ptr(mrb, self);
  return _fixnum_value(tm->datetime.tm_yday + 1);
}

/* 15.2.19.7.32 */
/* Returns year of time. */
static _value
_time_year(_state *mrb, _value self)
{
  struct _time *tm;

  tm = time_get_ptr(mrb, self);
  return _fixnum_value(tm->datetime.tm_year + 1900);
}

/* 15.2.19.7.33 */
/* Returns name of time's timezone. */
static _value
_time_zone(_state *mrb, _value self)
{
  struct _time *tm;

  tm = time_get_ptr(mrb, self);
  if (tm->timezone <= MRB_TIMEZONE_NONE) return _nil_value();
  if (tm->timezone >= MRB_TIMEZONE_LAST) return _nil_value();
  return _str_new_static(mrb,
                            timezone_names[tm->timezone].name,
                            timezone_names[tm->timezone].len);
}

/* 15.2.19.7.4 */
/* Returns a string that describes the time. */
static _value
_time_asctime(_state *mrb, _value self)
{
  struct _time *tm = time_get_ptr(mrb, self);
  struct tm *d = &tm->datetime;
  int len;

#if defined(MRB_DISABLE_STDIO)
  char *s;
# ifdef NO_ASCTIME_R
  s = asctime(d);
# else
  char buf[32];
  s = asctime_r(d, buf);
# endif
  len = strlen(s)-1;            /* truncate the last newline */
#else
  char buf[256];

  len = snprintf(buf, sizeof(buf), "%s %s %02d %02d:%02d:%02d %s%d",
    wday_names[d->tm_wday], mon_names[d->tm_mon], d->tm_mday,
    d->tm_hour, d->tm_min, d->tm_sec,
    tm->timezone == MRB_TIMEZONE_UTC ? "UTC " : "",
    d->tm_year + 1900);
#endif
  return _str_new(mrb, buf, len);
}

/* 15.2.19.7.6 */
/* Returns the day in the month of the time. */
static _value
_time_day(_state *mrb, _value self)
{
  struct _time *tm;

  tm = time_get_ptr(mrb, self);
  return _fixnum_value(tm->datetime.tm_mday);
}


/* 15.2.19.7.7 */
/* Returns true if daylight saving was applied for this time. */
static _value
_time_dst_p(_state *mrb, _value self)
{
  struct _time *tm;

  tm = time_get_ptr(mrb, self);
  return _bool_value(tm->datetime.tm_isdst);
}

/* 15.2.19.7.8 */
/* 15.2.19.7.10 */
/* Returns the Time object of the UTC(GMT) timezone. */
static _value
_time_getutc(_state *mrb, _value self)
{
  struct _time *tm, *tm2;

  tm = time_get_ptr(mrb, self);
  tm2 = (struct _time *)_malloc(mrb, sizeof(*tm));
  *tm2 = *tm;
  tm2->timezone = MRB_TIMEZONE_UTC;
  time_update_datetime(mrb, tm2);
  return _time_wrap(mrb, _obj_class(mrb, self), tm2);
}

/* 15.2.19.7.9 */
/* Returns the Time object of the LOCAL timezone. */
static _value
_time_getlocal(_state *mrb, _value self)
{
  struct _time *tm, *tm2;

  tm = time_get_ptr(mrb, self);
  tm2 = (struct _time *)_malloc(mrb, sizeof(*tm));
  *tm2 = *tm;
  tm2->timezone = MRB_TIMEZONE_LOCAL;
  time_update_datetime(mrb, tm2);
  return _time_wrap(mrb, _obj_class(mrb, self), tm2);
}

/* 15.2.19.7.15 */
/* Returns hour of time. */
static _value
_time_hour(_state *mrb, _value self)
{
  struct _time *tm;

  tm = time_get_ptr(mrb, self);
  return _fixnum_value(tm->datetime.tm_hour);
}

/* 15.2.19.7.16 */
/* Initializes a time by setting the amount of milliseconds since the epoch.*/
static _value
_time_initialize(_state *mrb, _value self)
{
  _int ayear = 0, amonth = 1, aday = 1, ahour = 0,
  amin = 0, asec = 0, ausec = 0;
  _int n;
  struct _time *tm;

  n = _get_args(mrb, "|iiiiiii",
       &ayear, &amonth, &aday, &ahour, &amin, &asec, &ausec);
  tm = (struct _time*)DATA_PTR(self);
  if (tm) {
    _free(mrb, tm);
  }
  _data_init(self, NULL, &_time_type);

  if (n == 0) {
    tm = current__time(mrb);
  }
  else {
    tm = time_mktime(mrb, ayear, amonth, aday, ahour, amin, asec, ausec, MRB_TIMEZONE_LOCAL);
  }
  _data_init(self, tm, &_time_type);
  return self;
}

/* 15.2.19.7.17(x) */
/* Initializes a copy of this time object. */
static _value
_time_initialize_copy(_state *mrb, _value copy)
{
  _value src;
  struct _time *t1, *t2;

  _get_args(mrb, "o", &src);
  if (_obj_equal(mrb, copy, src)) return copy;
  if (!_obj_is_instance_of(mrb, src, _obj_class(mrb, copy))) {
    _raise(mrb, E_TYPE_ERROR, "wrong argument class");
  }
  t1 = (struct _time *)DATA_PTR(copy);
  t2 = (struct _time *)DATA_PTR(src);
  if (!t2) {
    _raise(mrb, E_ARGUMENT_ERROR, "uninitialized time");
  }
  if (!t1) {
    t1 = (struct _time *)_malloc(mrb, sizeof(struct _time));
    _data_init(copy, t1, &_time_type);
  }
  *t1 = *t2;
  return copy;
}

/* 15.2.19.7.18 */
/* Sets the timezone attribute of the Time object to LOCAL. */
static _value
_time_localtime(_state *mrb, _value self)
{
  struct _time *tm;

  tm = time_get_ptr(mrb, self);
  tm->timezone = MRB_TIMEZONE_LOCAL;
  time_update_datetime(mrb, tm);
  return self;
}

/* 15.2.19.7.19 */
/* Returns day of month of time. */
static _value
_time_mday(_state *mrb, _value self)
{
  struct _time *tm;

  tm = time_get_ptr(mrb, self);
  return _fixnum_value(tm->datetime.tm_mday);
}

/* 15.2.19.7.20 */
/* Returns minutes of time. */
static _value
_time_min(_state *mrb, _value self)
{
  struct _time *tm;

  tm = time_get_ptr(mrb, self);
  return _fixnum_value(tm->datetime.tm_min);
}

/* 15.2.19.7.21 and 15.2.19.7.22 */
/* Returns month of time. */
static _value
_time_mon(_state *mrb, _value self)
{
  struct _time *tm;

  tm = time_get_ptr(mrb, self);
  return _fixnum_value(tm->datetime.tm_mon + 1);
}

/* 15.2.19.7.23 */
/* Returns seconds in minute of time. */
static _value
_time_sec(_state *mrb, _value self)
{
  struct _time *tm;

  tm = time_get_ptr(mrb, self);
  return _fixnum_value(tm->datetime.tm_sec);
}


/* 15.2.19.7.24 */
/* Returns a Float with the time since the epoch in seconds. */
static _value
_time_to_f(_state *mrb, _value self)
{
  struct _time *tm;

  tm = time_get_ptr(mrb, self);
  return _float_value(mrb, (_float)tm->sec + (_float)tm->usec/1.0e6);
}

/* 15.2.19.7.25 */
/* Returns a Fixnum with the time since the epoch in seconds. */
static _value
_time_to_i(_state *mrb, _value self)
{
  struct _time *tm;

  tm = time_get_ptr(mrb, self);
  if (tm->sec > MRB_INT_MAX || tm->sec < MRB_INT_MIN) {
    return _float_value(mrb, (_float)tm->sec);
  }
  return _fixnum_value((_int)tm->sec);
}

/* 15.2.19.7.26 */
/* Returns a Float with the time since the epoch in microseconds. */
static _value
_time_usec(_state *mrb, _value self)
{
  struct _time *tm;

  tm = time_get_ptr(mrb, self);
  if (tm->usec > MRB_INT_MAX || tm->usec < MRB_INT_MIN) {
    return _float_value(mrb, (_float)tm->usec);
  }
  return _fixnum_value((_int)tm->usec);
}

/* 15.2.19.7.27 */
/* Sets the timezone attribute of the Time object to UTC. */
static _value
_time_utc(_state *mrb, _value self)
{
  struct _time *tm;

  tm = time_get_ptr(mrb, self);
  tm->timezone = MRB_TIMEZONE_UTC;
  time_update_datetime(mrb, tm);
  return self;
}

/* 15.2.19.7.28 */
/* Returns true if this time is in the UTC timezone false if not. */
static _value
_time_utc_p(_state *mrb, _value self)
{
  struct _time *tm;

  tm = time_get_ptr(mrb, self);
  return _bool_value(tm->timezone == MRB_TIMEZONE_UTC);
}


void
_mruby_time_gem_init(_state* mrb)
{
  struct RClass *tc;
  /* ISO 15.2.19.2 */
  tc = _define_class(mrb, "Time", mrb->object_class);
  MRB_SET_INSTANCE_TT(tc, MRB_TT_DATA);
  _include_module(mrb, tc, _module_get(mrb, "Comparable"));
  _define_class_method(mrb, tc, "at", _time_at, MRB_ARGS_ARG(1, 1));      /* 15.2.19.6.1 */
  _define_class_method(mrb, tc, "gm", _time_gm, MRB_ARGS_ARG(1,6));       /* 15.2.19.6.2 */
  _define_class_method(mrb, tc, "local", _time_local, MRB_ARGS_ARG(1,6)); /* 15.2.19.6.3 */
  _define_class_method(mrb, tc, "mktime", _time_local, MRB_ARGS_ARG(1,6));/* 15.2.19.6.4 */
  _define_class_method(mrb, tc, "now", _time_now, MRB_ARGS_NONE());       /* 15.2.19.6.5 */
  _define_class_method(mrb, tc, "utc", _time_gm, MRB_ARGS_ARG(1,6));      /* 15.2.19.6.6 */

  _define_method(mrb, tc, "=="     , _time_eq     , MRB_ARGS_REQ(1));
  _define_method(mrb, tc, "<=>"    , _time_cmp    , MRB_ARGS_REQ(1)); /* 15.2.19.7.1 */
  _define_method(mrb, tc, "+"      , _time_plus   , MRB_ARGS_REQ(1)); /* 15.2.19.7.2 */
  _define_method(mrb, tc, "-"      , _time_minus  , MRB_ARGS_REQ(1)); /* 15.2.19.7.3 */
  _define_method(mrb, tc, "to_s"   , _time_asctime, MRB_ARGS_NONE());
  _define_method(mrb, tc, "inspect", _time_asctime, MRB_ARGS_NONE());
  _define_method(mrb, tc, "asctime", _time_asctime, MRB_ARGS_NONE()); /* 15.2.19.7.4 */
  _define_method(mrb, tc, "ctime"  , _time_asctime, MRB_ARGS_NONE()); /* 15.2.19.7.5 */
  _define_method(mrb, tc, "day"    , _time_day    , MRB_ARGS_NONE()); /* 15.2.19.7.6 */
  _define_method(mrb, tc, "dst?"   , _time_dst_p  , MRB_ARGS_NONE()); /* 15.2.19.7.7 */
  _define_method(mrb, tc, "getgm"  , _time_getutc , MRB_ARGS_NONE()); /* 15.2.19.7.8 */
  _define_method(mrb, tc, "getlocal",_time_getlocal,MRB_ARGS_NONE()); /* 15.2.19.7.9 */
  _define_method(mrb, tc, "getutc" , _time_getutc , MRB_ARGS_NONE()); /* 15.2.19.7.10 */
  _define_method(mrb, tc, "gmt?"   , _time_utc_p  , MRB_ARGS_NONE()); /* 15.2.19.7.11 */
  _define_method(mrb, tc, "gmtime" , _time_utc    , MRB_ARGS_NONE()); /* 15.2.19.7.13 */
  _define_method(mrb, tc, "hour"   , _time_hour, MRB_ARGS_NONE());    /* 15.2.19.7.15 */
  _define_method(mrb, tc, "localtime", _time_localtime, MRB_ARGS_NONE()); /* 15.2.19.7.18 */
  _define_method(mrb, tc, "mday"   , _time_mday, MRB_ARGS_NONE());    /* 15.2.19.7.19 */
  _define_method(mrb, tc, "min"    , _time_min, MRB_ARGS_NONE());     /* 15.2.19.7.20 */

  _define_method(mrb, tc, "mon"  , _time_mon, MRB_ARGS_NONE());       /* 15.2.19.7.21 */
  _define_method(mrb, tc, "month", _time_mon, MRB_ARGS_NONE());       /* 15.2.19.7.22 */

  _define_method(mrb, tc, "sec" , _time_sec, MRB_ARGS_NONE());        /* 15.2.19.7.23 */
  _define_method(mrb, tc, "to_i", _time_to_i, MRB_ARGS_NONE());       /* 15.2.19.7.25 */
  _define_method(mrb, tc, "to_f", _time_to_f, MRB_ARGS_NONE());       /* 15.2.19.7.24 */
  _define_method(mrb, tc, "usec", _time_usec, MRB_ARGS_NONE());       /* 15.2.19.7.26 */
  _define_method(mrb, tc, "utc" , _time_utc, MRB_ARGS_NONE());        /* 15.2.19.7.27 */
  _define_method(mrb, tc, "utc?", _time_utc_p,MRB_ARGS_NONE());       /* 15.2.19.7.28 */
  _define_method(mrb, tc, "wday", _time_wday, MRB_ARGS_NONE());       /* 15.2.19.7.30 */
  _define_method(mrb, tc, "yday", _time_yday, MRB_ARGS_NONE());       /* 15.2.19.7.31 */
  _define_method(mrb, tc, "year", _time_year, MRB_ARGS_NONE());       /* 15.2.19.7.32 */
  _define_method(mrb, tc, "zone", _time_zone, MRB_ARGS_NONE());       /* 15.2.19.7.33 */

  _define_method(mrb, tc, "initialize", _time_initialize, MRB_ARGS_REQ(1)); /* 15.2.19.7.16 */
  _define_method(mrb, tc, "initialize_copy", _time_initialize_copy, MRB_ARGS_REQ(1)); /* 15.2.19.7.17 */

  /*
    methods not available:
      gmt_offset(15.2.19.7.12)
      gmtoff(15.2.19.7.14)
      utc_offset(15.2.19.7.29)
  */
}

void
_mruby_time_gem_final(_state* mrb)
{
}
