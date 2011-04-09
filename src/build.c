
#include <stdlib.h>

#include "packrat.h"
#include "control.h"
#include "require.h"
#include "debug.h"

int packrat_build(const char * path)
{
  char lha[256]; 
  struct packrat_control * ctrl = packrat_parse_control("../PACKAGE/control"); /* FIXME */
  if (!ctrl) {
	fprintf(stderr,"FATAL: Unable to parse package control file\n");
	return 1;
  }

  D(packrat_debug_control(ctrl));

  /* We need c:lha to be able to pack the archive */
  if (!packrat_find_file("c:lha", lha, sizeof(lha))) {
	fprintf(stderr,"FATAL: Packrat requires lha to be availble in C: (or $PATH on non-Amiga systems)\n");
	return 1;
  }

  packrat_free_control(ctrl);
  return 0;
}
