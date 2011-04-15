#include <string.h>
#include <stdio.h>
#include "packrat.h"

void usage()
{
  printf("\n");
  printf("packrat version %s (C) 2011 Vidar Hokstad <vidar@hokstad.com> \n\n",VERSION);
  printf("Usage:\n"
		 "   packrat [global options] [command] [command specific options]\n"
		 "\n"
		 "GLOBAL OPTIONS\n"
		 "   -v, /v, verbose       Turn on debug output\n"
		 "   -h, -?, ?, help       This help text\n"
		 "\n"
		 "COMMANDS\n"
		 "   install [package]     Install the package\n"
		 "   remove [package]      Uninstall the package \n"
		 "   query [package]       Show information about the specified package\n"
		 "   list [pattern]        List installed packages and other resources\n"
		 "                         (libraries etc.) matching the (optional) pattern\n"
         "   verify [pattern]      Verify the package(s) matching [pattern]\n"
         "   scan                  Scan system for available packages and resources\n"
		 "\n" 
         "   For developers:\n"
		 "\n"
		 "   createpackage [path]  Create a package skeleton on [path]\n"
		 "   build [path]          Build a package from [path]\n"
		 "\n");
}

int main(int argc, const char ** argv)
{
  const char * opt;
  short verbose = 0;
  ++argv; --argc;
  while (argc > 0) {
	if (!strcasecmp(*argv,"verbose")) verbose = 1;
	else if (!strcasecmp(*argv,"help") || !strcmp(*argv,"?")) {
	  usage();
	  return 0;
	} else if (*argv[0] == '/' || *argv[0] == '-') {
	  opt = *argv;
	  ++opt;
	  while (*opt) {
		switch (*opt) {
		case 'h':
		case '?':
		  usage();
		  return 0;
		case 'v':
		  verbose = 1;
		  break;
		default:
		  fprintf(stderr,"Unknown global option: '%c'\n", *opt);
		  return 1;
		}
		++opt;
	  }
	} else break;
	++argv; --argc;
  }

  if (argc == 0) {
	usage();
	return 0;
  }

  if (!strcasecmp(*argv,"build")) {
	return packrat_build(argv[1]);
  } else {
	fprintf(stderr,"ERROR: Unknown / unimplemented command '%s'\n",*argv);
	return 1;
  }

  return 0;
}
