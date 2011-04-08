
#include "packrat.h"
#include "control.h"
#include "require.h"
#include "debug.h"

void packrat_build(const char * path)
{
  struct packrat_control * ctrl = packrat_parse_control("../PACKAGE/control"); /* FIXME */
  if (!ctrl) return;

  packrat_debug_control(ctrl);

  if (packrat_find_object("c:lha", PACKRAT_CMP_NONE,0)) {
	Log("Found c:lha");
  }

  packrat_free_control(ctrl);
}
