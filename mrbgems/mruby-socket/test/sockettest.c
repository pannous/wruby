#include <stdio.h>
#include <stdlib.h>

#include "mruby.h"
#include "mruby/error.h"

#if defined(_WIN32) || defined(_WIN64)

#include <io.h>

#ifdef _MSC_VER

#include <fcntl.h>
#include <sys/stat.h>
#define open  _open
#define close _close
#define unlink _unlink

static int
mkstemp(char *p)
{
  int fd;
  char* fname = _mktemp(p);
  if (fname == NULL)
    return -1;
  fd = open(fname, O_RDWR | O_CREAT | O_EXCL, _S_IREAD | _S_IWRITE);
  if (fd >= 0)
    return fd;
  return -1;
}
#endif

#else

#include <unistd.h>

#endif

value
sockettest_tmppath(state *mrb, value klass)
{
  char name[] = "mruby-socket.XXXXXXXX";
  int fd = mkstemp(name);
  if (fd == -1) {
    sys_fail(mrb, 0);
  }
  if (close(fd) == -1) {
    sys_fail(mrb, 0);
  }
  if (unlink(name) == -1) {
    sys_fail(mrb, 0);
  }
  return str_new_cstr(mrb, name);
}

value
sockettest_win_p(state *mrb, value klass)
{
#ifdef _WIN32
  return true_value();
#else
  return false_value();
#endif
}

value
sockettest_cygwin_p(state *mrb, value klass)
{
#if defined(__CYGWIN__) || defined(__CYGWIN32__)
  return true_value();
#else
  return false_value();
#endif
}

void
mruby_socket_gem_test(state* mrb)
{
  struct RClass *c = define_module(mrb, "SocketTest");
  define_class_method(mrb, c, "tmppath", sockettest_tmppath, ARGS_NONE());
  define_class_method(mrb, c, "win?", sockettest_win_p, ARGS_NONE());
  define_class_method(mrb, c, "cygwin?", sockettest_cygwin_p, ARGS_NONE());
}
