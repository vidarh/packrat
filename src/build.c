
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

  /* FIXME: Verify the PACKAGE directory */


  /* Check the BuildRequires */
  struct packrat_node * node;
  FOREACH_ML(&(ctrl->buildrequires),node) {
	/* FIXME: Version check */
	if (!packrat_find_object(node->str,PACKRAT_CMP_NONE,0)) {
	  fprintf(stderr,"FATAL: Package requires '%s' for build\n", node->str);
	  return 1;
	}
  }

  /* Copy the %%build section to T: w/ debug echo's */
  FOREACH_ML(&(ctrl->build),node) {
	fprintf(stderr,"BUILD: %s\n",node->str);
  }


  /* Create a build root in T: */

  /* Chdir to T: */

  /* Execute the build script */

  /* Create a MANIFEST with md5sums */

  /* Create the INSTALL file */

  /* Create an Amiga Installer and uninstall script */

  /* Package up build root in an lha archive */
  
  packrat_free_control(ctrl);
  return 0;
}
