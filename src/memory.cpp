extern "C" {
#include <stdio.h>
#include <stdlib.h>
}
#ifdef LEAK_CHECK

#endif

#include "memory.h"


void *dc_free(void * ptr)
{
	if(ptr)
	{
		delete ptr;
	}
	return NULL;
}
