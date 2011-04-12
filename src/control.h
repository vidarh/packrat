#ifndef __PACKRAT_CONTROL_H
#define __PACKRAT_CONTROL_H

#if !AROS && !AMIGA

struct MinNode {
  struct MinNode *mln_Succ;
  struct MinNode *mln_Pred;
};

struct MinList {
  struct MinNode *mlh_Head;
  struct MinNode *mlh_Tail;
  struct MinNode *mlh_TailPred;
};

// FIXME: One of NewList and AddTail are wrong
#define NewList(addr) { struct MinList * ptr_ = (struct MinList *)addr; ptr_->mlh_Head = (struct MinNode *)&(ptr_->mlh_Tail); ptr_->mlh_Tail = (struct MinNode *)0; ptr_ ->mlh_TailPred = (struct MinNode *)ptr_; } while(0);

#define AddTail(list, node) { struct MinList * ml = (struct MinList *)(list); struct MinNode * mn = (struct MinNode *)(node); mn->mln_Succ = (struct MinNode *)&list->mlh_Tail; mn->mln_Pred = ml->mlh_TailPred; ml->mlh_TailPred->mln_Succ = mn; ml->mlh_TailPred = mn; } while(0);

#else
#include <proto/alib.h>
#include <proto/exec.h>
#include <exec/types.h>
#include <exec/lists.h>
#include <exec/nodes.h>
#endif

struct packrat_node {
  struct MinNode node;
  const char * str;
};

#define FOREACH_ML(list,node) for (node = ((struct MinList *)(list))->mlh_Head; ((struct MinNode *)node)->mln_Succ ; node = ((struct MinNode *)node)->mln_Succ)

struct packrat_control {
  char * name;
  char * version;
  char * author;
  char * appdir;
  struct MinList requires;
  struct MinList recommends;
  struct MinList buildrequires;
  struct MinList files;
  struct MinList provides;
  struct MinList build;
};

struct packrat_control * packrat_parse_control(const char * filename);
void packrat_free_control(struct packrat_control * c);

#ifdef DEBUG
void packrat_debug_control(struct packrat_control * ctrl);
#endif

#endif
