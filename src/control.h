#ifndef __PACKRAT_CONTROL_H
#define __PACKRAT_CONTROL_H

#if !AROS && !AMIGA
struct Node {
};

struct List {
};
#else
#include <proto/alib.h>
#include <proto/exec.h>
#include <exec/types.h>
#include <exec/lists.h>
#include <exec/nodes.h>
#endif

struct packrat_control {
  char * name;
  char * version;
  char * author;
  char * appdir;
  struct List requires;
  struct List recommends;
  struct List buildrequires;
  struct List files;
  struct List provides;
  struct List build;
};

struct packrat_control * packrat_parse_control(const char * filename);
void packrat_free_control(struct packrat_control * c);

#endif
