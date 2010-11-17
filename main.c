#include "mem_map.h"

static volatile int g_counter = 0x33333333;

void main( void )
{
	while(1) {
		g_counter++;
	}
}
