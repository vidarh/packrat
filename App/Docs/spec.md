
Packrat Specification v0.1
==========================

Introduction
------------

Packrat is an Amiga-centric but portable package manager.
It is designed to be:

 * Portable: It should be easy to build and run on AROS, AmigaOS 3.x,
   AmigaOS 4.x, MorphOS and Linux.
 * Well behaved on Amiga-platforms: Respect conventions and user choices,
   and don't step on manual changes.
 * Non-intrusive: Packrat packages should be installable without
   packrat, and users should not need to change behaviour unless
   they want to.
 * Simple to use.
 * Self-contained: Packrat includes all its dependencies other than
   lha.
 * Compact

A packrat package is a standard lha archive. It is required to be
in a format supported by lha 2.xx and Unix lha 1.14 (the default
options of either will produce working packages).

The lha archive contains files structured as follows:

 * PACKAGE/
   * control
   * postinst
   * prerm
   * startup
 * $$AppDir$$ (path found in PACKAGE/control under the "AppDir:" directive; defaults to "App" if not listed)
 * $$AppDir$$.info (optional, but recommended to faciliatet manual installation
 * EnvArc            -- Contains default configuration that will go into EnvArc:$$AppDir$$ (optional)
 * $$AppDir$$/Docs   -- Optional. If it exists, it will be marked as documentation (and optionally moved to a centralized doc repository)
 * MANIFEST(.info)   -- List of the included files
 * INSTALL(.info)    -- Instructions for manual installation, generated by Packrat
 * .. optionally any system directory with files to install to, such as Libs 
 
A Packrat *source archive* is a package of the same structure except the
PACKAGE/control file contains a "%%build" and "%%files" section that specifies
how to build the package, and  optionally a "src" directory containing the
source or a pristine source archive. The source archive may also lack $$AppDir$$,
MANIFEST/INSTALL, the former must be created by the build code, the latter will
be auto-generated.

To create a skeleton for a new package, you may use:

    packrat createpackage [path]
   

Building packages
-----------------

When executing

    packrat build [path]

Packrat will try to build a package. If "path" is not provided, it will look in the current working
directory.

The build process will first validate the PACKAGE directory, then check for any build requirements
listed in PACKAGE/control. Finally it will copy the package to a temporary directory in T: (the build root),
write PACKAGE/control's %%build section to T: and execute it as a script with the packages build root
as the current directory. 

It will also generate a MANIFEST file that lists all the files in the archive with their md5sums,
and an INSTALL file that contains English textual installation instructions and a description of 
Packrat. Future improvements may include auto-creating an installer script.

If successful, it will then use the file patterns in %%files to package up an lha archive using the
"lha" program. Packrat will by default exclude any Unix-style backup files (ending in "~") and files
in ".cvs",".svn" and ".git" directories.



Installing packages
-------------------

To install a package, use:

    packrat install [path]
	
This can currently only be the path of a local package. In the future packrat will gain
support for remote repositories.

The installation proceeds as follows:

 * Packrat uses "lha x" to unpack the archive in a temporary location.
 * Packrat sanity checks the package by checking the md5sums in the MANIFEST file.
 * Packrat then reads the control file and verifies whether or not the required and
   recommended packages are there. 
 * If the required packages are not there, installation will halt.
 * If recommended packages are not there, packrat will ask whether or not to proceed.
 * Packrat will then copy the files to the appropriate locations, and update its 
   database.
 * Packrat will create an install log and uninstall script and place it in the
   package directory for users that may wish to manually uninstall.
 * If any system files needs to be overwritten, Packrat will check its database for
   conflicts, and if so give warnings/errors. It will also regardless take backups
   of overwritten files.
   