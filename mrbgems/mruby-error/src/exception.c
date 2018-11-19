#include <mruby.h>
#include <mruby/throw.h>
#include <mruby/error.h>

MRB_API _value
_protect(_state *mrb, _func_t body, _value data, _bool *state)
{
  struct _jmpbuf *prev_jmp = mrb->jmp;
  struct _jmpbuf c_jmp;
  _value result = _nil_value();

  if (state) { *state = FALSE; }

  MRB_TRY(&c_jmp) {
    mrb->jmp = &c_jmp;
    result = body(mrb, data);
    mrb->jmp = prev_jmp;
  } MRB_CATCH(&c_jmp) {
    mrb->jmp = prev_jmp;
    result = _obj_value(mrb->exc);
    mrb->exc = NULL;
    if (state) { *state = TRUE; }
  } MRB_END_EXC(&c_jmp);

  _gc_protect(mrb, result);
  return result;
}

MRB_API _value
_ensure(_state *mrb, _func_t body, _value b_data, _func_t ensure, _value e_data)
{
  struct _jmpbuf *prev_jmp = mrb->jmp;
  struct _jmpbuf c_jmp;
  _value result;

  MRB_TRY(&c_jmp) {
    mrb->jmp = &c_jmp;
    result = body(mrb, b_data);
    mrb->jmp = prev_jmp;
  } MRB_CATCH(&c_jmp) {
    mrb->jmp = prev_jmp;
    ensure(mrb, e_data);
    MRB_THROW(mrb->jmp); /* rethrow catched exceptions */
  } MRB_END_EXC(&c_jmp);

  ensure(mrb, e_data);
  _gc_protect(mrb, result);
  return result;
}

MRB_API _value
_rescue(_state *mrb, _func_t body, _value b_data,
           _func_t rescue, _value r_data)
{
  return _rescue_exceptions(mrb, body, b_data, rescue, r_data, 1, &mrb->eStandardError_class);
}

MRB_API _value
_rescue_exceptions(_state *mrb, _func_t body, _value b_data, _func_t rescue, _value r_data,
                      _int len, struct RClass **classes)
{
  struct _jmpbuf *prev_jmp = mrb->jmp;
  struct _jmpbuf c_jmp;
  _value result;
  _bool error_matched = FALSE;
  _int i;

  MRB_TRY(&c_jmp) {
    mrb->jmp = &c_jmp;
    result = body(mrb, b_data);
    mrb->jmp = prev_jmp;
  } MRB_CATCH(&c_jmp) {
    mrb->jmp = prev_jmp;

    for (i = 0; i < len; ++i) {
      if (_obj_is_kind_of(mrb, _obj_value(mrb->exc), classes[i])) {
        error_matched = TRUE;
        break;
      }
    }

    if (!error_matched) { MRB_THROW(mrb->jmp); }

    mrb->exc = NULL;
    result = rescue(mrb, r_data);
  } MRB_END_EXC(&c_jmp);

  _gc_protect(mrb, result);
  return result;
}

void
_mruby_error_gem_init(_state *mrb)
{
}

void
_mruby_error_gem_final(_state *mrb)
{
}
