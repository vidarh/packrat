
#include <stdlib.h>
#include <string.h>

#include "packrat.h"
#include "control.h"
#include "require.h"
#include "debug.h"

#define PATHSIZE 256

#ifdef __AMIGA__
#define TMPPATH "T:"
#else
#define TMPPATH "/tmp/"
#endif

int packrat_build(const char * path)
{
  char lha[PATHSIZE]; 
  struct packrat_control * ctrl = packrat_parse_control("../PACKAGE/control"); /* FIXME */
  if (!ctrl) {
	fprintf(stderr,"FATAL: Unable to parse package control file\n");
	return 1;
  }

  D(packrat_debug_control(ctrl));

  /* We need c:lha to be able to pack the archive */
  if (!packrat_find_file("c:lha", lha, sizeof(lha))) {
	fprintf(stderr,"FATAL: Packrat requires lha to be availble in C: (or $PATH on non-Amiga systems)\n");
	goto fail;
  }

  /* FIXME: Verify the PACKAGE directory */


  /* Check the BuildRequires */
  struct packrat_node * node;
  FOREACH_ML(&(ctrl->buildrequires),node) {
	/* FIXME: Version check */
	if (!packrat_find_object(node->str,PACKRAT_CMP_NONE,0)) {
	  fprintf(stderr,"FATAL: Package requires '%s' for build\n", node->str);
	  goto fail;
	}
  }

  /* Copy the %%build section to T: w/ debug echo's */
  char tmpfile[PATHSIZE];
  /* FIXME: Create a unique path */
  strncpy(tmpfile,TMPPATH"build",sizeof(tmpfile));
  FILE * buildfile = fopen(tmpfile,"w");
  if (!buildfile) {
	fprintf(stderr,"FATAL: Unable to open temporary file for buid '%s'\n",tmpfile);
	goto fail;
  }
  FOREACH_ML(&(ctrl->build),node) {
	/* FIXME: Apply variable substitutions */
	fprintf(buildfile,"%s\n",node->str);
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

 fail:
  packrat_free_control(ctrl);
  return 1;
}
