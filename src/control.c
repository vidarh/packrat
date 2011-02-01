
/* Simple parser for the PACKAGE/control file */

#include "control.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static char * rtrim_c(char * pos, short keepcomment) {
  char * value = pos;
  while (*pos && *pos != '\r' && *pos != '\n' && (keepcomment || *pos != '#')) ++pos;
  while (pos > value && isspace(pos[-1])) --pos;
  *pos = 0;
  return value;
}

static inline char * rtrim(char * pos) { return rtrim_c(pos,0); }

static char * ltrim(char * pos) {
  while (isspace(*pos)) ++pos;
  if (*pos == '#') while(*pos) ++pos;
  return pos;
}

static signed int packrat_parse_control_list(struct List * list, char * linebuf, short * lastcomma, short plist)
{
  short len;
  char * entry;

  if ((plist && strncmp(linebuf,"%%",2)) ||
	  (isspace(linebuf[0]) || *lastcomma || linebuf[0] == 0 || linebuf[0] == '#')) {
	rtrim_c(linebuf,plist);
	len = strlen(linebuf);
	*lastcomma =  len > 0 && linebuf[len-1]  == ',';
	if (!plist) linebuf = ltrim(linebuf);
	
	if (!plist) {
	  entry = strtok(rtrim(ltrim(linebuf)),",");
	  while(entry) {
		entry = rtrim(ltrim(entry));
		if (entry[0]) fprintf(stderr,"DEBUG: -> %s\n",entry);
		entry = strtok(0,",");
	  }
	} else {
	  if (linebuf[0]) fprintf(stderr,"DEBUG: -> %s\n",linebuf);
	}
	return 1;
  }
  return 0;
}

static signed int packrat_parse_control_headers(FILE * f,struct packrat_control * ctrl)
{
  char linebuf[1024];
  const char * name;
  struct List * inlist = 0;
  short plist = 0;
  short lastcomma = 0;

  while (fgets(linebuf,sizeof(linebuf),f)) {
	char * pos = linebuf;

	if (inlist) {
	  if (packrat_parse_control_list(inlist, linebuf,&lastcomma,plist)) continue;
	  inlist = 0;
	}
  
	while (isspace(*pos)) ++pos;
	
	name = pos;
	
	if (name[0] == '%' && name[1] == '%') {
	  name += 2;
	  pos += 2;
	  plist = 1;
	} else (plist = 0);

	if (*pos =='\n' || *pos =='\r' || *pos == '#' || !*pos) {
	  if (plist) {
		plist = 0;
		inlist = 0;
	  } else continue;
	}

	while (isalpha(*pos)) ++pos;
	if (!plist && *pos != ':') {
	  fprintf(stderr, "Syntax error: Expected ':': '%s'\n",linebuf);
	  return -1;
	}
	*pos = 0;
	fprintf(stderr,"DEBUG: %s\n",name);
	++pos;

	pos = ltrim(pos);
	rtrim(pos);
	
	if (!strcasecmp(name,"Name")) ctrl->name = strdup(pos);
	else if (!strcasecmp(name,"Version")) ctrl->version = strdup(pos);
	else if (!strcasecmp(name,"Author"))  ctrl->author = strdup(pos);
	else if (!strcasecmp(name,"AppDir"))  ctrl->appdir = strdup(pos);
	else {
	  if (!strcasecmp(name,"Requires")) inlist = &ctrl->requires;
	  else if (!strcasecmp(name,"BuildRequires")) inlist = &ctrl->buildrequires;
	  else if (!strcasecmp(name,"Files")) inlist = &ctrl->files;
	  else if (!strcasecmp(name,"Build")) inlist = &ctrl->build;
	  else if (!strcasecmp(name,"Provides")) inlist = &ctrl->provides;
	  else if (!strcasecmp(name,"Recommends")) inlist = &ctrl->recommends;
	  else {
		fprintf(stderr,"Unknown header '%s'\n");
		return 0;
	  }

	  if (inlist) {
		/* Handle the first line (end of the initial line) */
		lastcomma = 1; /* We fake this to ensure the first line bit is considered */
		if (packrat_parse_control_list(inlist, pos,&lastcomma,plist)) continue;
		inlist = 0;
	  }
	}
  }
  /* FIXME update docs: Treat Requires, Recommends, BuildRequires, Files and Provides as lists.
	 Lists are one entry per line (possibly including the header line).

	 If a list is introduced with '%%[headername]', only %% or '%%[another header name]'
     can terminate it, and values can start in the first column. No value can start on the
	 "%%" line (the space after %%[headername] is reserved for conditionals, such as
	 restricting the list to certain architectures).

	 If a list is introduced with '[headername]:', each line in the list must start with space or tab,
	 and either comma or LF indicates a new value, and the values can start immediately after the ':'
  */

  return 1;
}

void packrat_debug_control(struct packrat_control * ctrl)
{
  fprintf(stderr,"Name: %s\n",ctrl->name);
  fprintf(stderr,"Version: %s\n",ctrl->version);
  fprintf(stderr,"Author: %s\n",ctrl->author);
  fprintf(stderr,"AppDir: %s\n",ctrl->appdir);
}

struct packrat_control * packrat_parse_control(const char * filename)
{
  struct packrat_control * ctrl;
  FILE * f = fopen(filename,"r");
  if (!f) return 0;

  ctrl = calloc(1,sizeof(struct packrat_control)); /* FIXME: Does AROS malloc use  RT? */
  if (ctrl) {
	if (!packrat_parse_control_headers(f,ctrl)) {
	  packrat_free_control(ctrl);
	  return 0;
	}
  }

  /* Defaults */
  if (!ctrl->appdir) ctrl->appdir = strdup("App");

  fclose(f);
  return ctrl;
}

void packrat_free_control(struct packrat_control * c)
{
  if (!c) return;
  if (c->name) free(c->name);
  if (c->version) free(c->version);
  if (c->author) free(c->author);
  if (c->appdir) free(c->appdir);
  free(c);
}

