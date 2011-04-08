#include <string.h>

#include "debug.h"
#include "require.h"

 #include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

static int file_exists(const char * object)
{
  struct stat statbuf;
  Log(object)
#ifdef AMIGA
	Log("file_exists not implemented")
#else
	if (!stat(object,&statbuf)) return 1;
#endif
  return 0;
}

/* FIXME: Make sure not to trigger requesters on AmigaOS unnecessarily. */
static int packrat_find_file(const char * object, char * real_path, int path_size)
{
  Log(object)
  if (file_exists(object)) {
	strncpy(real_path,object,path_size);
	return 1;
  }

#ifndef AMIGA
  /* FIXME: If path is C: or LIBS:, we rewrite so that dependencies such as
	 C:lha will work
  */
  if (!strncasecmp("c:",object,2)) {
	const char * path = getenv("PACKRAT_PATH");
	if (!path) path = getenv("PATH");
	if (path) {
	  char * saveptr;
	  char * chk;
	  path = strdup(path);
	  Log("Attempting rewrite using paths:");
	  Log(path);
	  chk = strtok_r(path,":",&saveptr);
	  do {
		snprintf(real_path,path_size,"%s/%s",chk, object + 2);
		if (file_exists(real_path)) {
		  Log(" FOUND!");
		  free(path);
		  return 1;
		}
	  } while (chk = strtok_r(0,":",&saveptr));
	  free(path);
	}
  } else if (!strncasecmp("libs:",object,5)) {
	Log("libs: not yet rewritten");
  }
#endif
  
  /* FIXME: If this is not an absolute path (starts with an assign or volume),
   * we iterate over PATH and PACKRAT_PATH
   */

  return 0;
}


static int packrat_find_package(const char * object)
{
  Log(object)
  return 0;
}

int packrat_find_object(const char * object, unsigned short cmpop, const char * version)
{
  Log(object);

  /* FIXME: 
   * 
   * If path is specified:
   *   - If file exist, check version
   *   - If path is C: or LIBS: and on non-Amiga OS, check
   *     $PATH if C:, and /lib, /usr/lib, and /usr/local/lib otherwise
   * If path is not specified:
   *   - Iterate over PATH and PACKRAT_PATH (if set)
   *
   * If no file was found, check the package database.
   *
   * If no object was found, fail.
   *
   * Otherwise, compare the version string according to cmpop
   *
   */

  char real_path[1024];
  
  if (packrat_find_file(object, real_path,sizeof(real_path))) {
	if (cmpop == PACKRAT_CMP_NONE) return 1;
	Log("Version check not yet implemented");
  } else if (packrat_find_package(object)) {
  } else {
	return 0;
  }

  /* FIXME: Compare version */

  return 0;
}
