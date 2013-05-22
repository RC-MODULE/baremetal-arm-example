// 1 or 2 seconds on 300 MHz
#include "uemd_early.h"
#define DELAY (30*10000)
#ifndef bool
#define bool int
#define false 0
#define true 1
#endif
#define PHYS_IM0              0x00100000
struct uemd_otp g_otp;


/* rom stuff */
void (*nand_init)(struct uemd_nand_state *state);
int (*nand_read)(struct uemd_nand_state *state, int start, void *dest, int words_len);
int (*otp_create)(struct uemd_otp *otp);


/* NAND layout */
/* 0: checksum */
/* 16: nanoloader */
/* 4K: uboot */
/* 256K: uboot-env */

void main( void )
{
	volatile int i,k;
	/* hook rom functions */
	for (k=0;k<2;k++)
	{
	*((long int*)0x2002a3fc)=0x40;
	for(i=0; i<DELAY; i++);
	*((long int*)0x2002a3fc)=0x00;
	for(i=0; i<DELAY; i++);
	}
	nand_init = 0x000007e0;
	nand_read = 0x000009f4;
	otp_create = 0x00001978;
	otp_create(&g_otp);
	uemd_em_init();
	struct uemd_nand_state nand;
	nand_init(&nand);
	/* Now let's load up u-boot from 0x400*/
 	nand_read(&nand, 0x800, (void *)(0x47000000), 0x30000/sizeof(unsigned int));
	/* No return from where we're going */
	uemd_longjump((void *)(0x47000000));
// 	/* This will not get executed evar */
}
