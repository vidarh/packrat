
#include "packrat.h"
#include "control.h"

void packrat_build(const char * path)
{
  struct packrat_control * ctrl = packrat_parse_control("../PACKAGE/control"); /* FIXME */
  if (!ctrl) return;

  packrat_debug_control(ctrl);
  packrat_free_control(ctrl);
}
