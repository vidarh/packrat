#ifndef __PACKRAT_REQUIRE_H__
#define __PACKRAT_REQUIRE_H__

/**
 *   Dependency management functions 
 *
 *   packrat_find_object(object, cmpop, versionstring)
 *     Checks whether the specificed object (given as a file name or package name)
 *     exists, and matches the given version string and comparison.
 *
 *     If the object is a file, packrat employs the following methods to determine
 *     the version number:
 *     * First scan for $VER: 
 *     * Failing that, scan for an RTC_MATCHWORD (romtag header for libraries etc.)
 *     * Failing that, if running on AmigaOS/MOS/AROS, try calling OpenLibrary() and
 *       examine the library base returned, if any.
 *
 *     FIXME: Consider whether to support querying rpm/dpkg when running on Linux,
 *     at least to support build dependencies for cross compilation.
 *
 *   ## Object names ##
 * 
 *   * If the object name is a filesystem path, that path is considered first
 *   * If the object matches a package name registered with Packrat, that package
 *     is considered first
 *   * If the name matches a file in Libs: or C: those paths are considered.
 *   * Otherwise Packrat does not know the object. Future versions will attempt
 *     remote lookups.
 *
 *   When running on non-Amiga compatible OS's, the following substitutions will
 *   be made when checking paths: C: will be expanded to check /bin, /sbin, /usr/local/bin,
 *   /usr/local/sbin and /opt/bin.
 *   
 *   FIXME: Consider if it's worth supporting checking for libraries on Linux too,
 *   though that would require rewriting/mangling names much more.
 *   
 *   ## Version strings ##
 * 
 *   The comparison operators >,<,= will work as expected. When comparing dotted 
 *   strings with numbers present, the numbers will be sorted numerically. E.g.
 *   1.10 > 1.9 is _true_.
 */

enum {
  PACKRAT_CMP_LT,
  PACKRAT_CMP_LE,
  PACKRAT_CMP_EQ,
  PACKRAT_CMP_NE,
  PACKRAT_CMP_GT,
  PACKRAT_CMP_GE,
  PACKRAT_CMP_NONE
};

int packrat_find_object(const char * object, unsigned short cmpop, const char * version);

#endif
