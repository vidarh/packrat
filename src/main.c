
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

int main()
{
  usage();
  return 0;
}
