#ifndef __PACKRAT_DEBUG_H

#include <stdio.h>

#ifdef DEBUG
#define D(expr) expr
#define Log(str) fprintf(stderr,"[%s @ %d/%s] ==> %s\n",__FILE__,__LINE__,__func__,str);
#else
#define D(expr)
#define Log(str)
#endif

#endif
