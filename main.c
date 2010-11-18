// 1 or 2 seconds on 300 MHz
#define DELAY (30*100000)

void main( void )
{
	volatile int i;
	while(1) {
		*((long int*)0x2002a3fc)=0x40;
		for(i=0; i<DELAY; i++);
		*((long int*)0x2002a3fc)=0x00;
		for(i=0; i<DELAY; i++);
	}
}
