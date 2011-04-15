
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <errno.h>
#include <unistd.h>

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

int execute_script(const char * script)
{
#ifdef __AMIGA__
#warning Make it execute
#else
  chmod(script,S_IXUSR|S_IRUSR|S_IWUSR);
  return system(script);
#endif
}

int packrat_build(const char * path)
{
  /* FIXME: There are two possibilities:
   * Either path points to an existing unpacked directory, in which case we need to copy that to the buildroot,
   * OR path points to an lha archive, in which case we can create the build root first and then unpack.
   * The simplest alternative might be to always assume a lha archive, and optionally *create a temporary one*
   * if it's not.
   *
   * Currently we're assuming an unpacked directory, but that needs to be fixed.
   */

  /* We need c:lha to be able to pack the archive */
  char lha[PATHSIZE]; 
  if (!packrat_find_file("c:lha", lha, sizeof(lha))) {
	fprintf(stderr,"FATAL: Packrat requires lha to be availble in C: (or $PATH on non-Amiga systems)\n");
	goto fail;
  }

  char packagedir[PATHSIZE];
  snprintf(packagedir, sizeof(packagedir),"%s/PACKAGE",path);

  char controlname[PATHSIZE];
  snprintf(controlname, sizeof(controlname),"%s/control",packagedir);

  struct packrat_control * ctrl = packrat_parse_control(controlname);
  if (!ctrl) {
	fprintf(stderr,"FATAL: Unable to parse package control file '%s'\n",controlname);
	return 1;
  }

  D(packrat_debug_control(ctrl));

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

  /* --- Copy the %%build section to T: w/ debug echo's */

  char tmpfile[PATHSIZE];
  /* FIXME: Create a unique path */
  strncpy(tmpfile,TMPPATH"build",sizeof(tmpfile));
  FILE * buildfile = fopen(tmpfile,"w");
  if (!buildfile) {
	fprintf(stderr,"FATAL: Unable to open temporary file for build '%s'\n",tmpfile);
	goto fail;
  }
  FOREACH_ML(&(ctrl->build),node) {
	/* FIXME: Apply variable substitutions */
	fprintf(buildfile,"echo %s\n",node->str);
	fprintf(buildfile,"%s\n",node->str);
  }
  fclose(buildfile);

  /* Create a build root in TMPPATH */
  char buildroot[PATHSIZE];
  strncpy(buildroot,TMPPATH"builddir",sizeof(buildroot));

  if (mkdir(buildroot, S_IRUSR|S_IWUSR|S_IXUSR)) {
	fprintf(stderr,"FATAL: Unable to create buildroot '%s'\n",buildroot);
	goto fail;
  }

  if (chdir(buildroot)) {
	fprintf(stderr,"FATAL: Unable to chdir to buildroot '%s'\n",buildroot);
	goto fail;
  }

  /* Copy the directory or archive into it */

  
  execute_script(tmpfile);

  /* Create a MANIFEST with md5sums */

  /* Create the INSTALL file */

  /* Create an Amiga Installer and uninstall script */

  /* Package up build root in an lha archive */
  
  packrat_free_control(ctrl);
  return 0;

 fail:
  fprintf(stderr,"ERROR: %s\n",strerror(errno));
  packrat_free_control(ctrl);
  return 1;
}
