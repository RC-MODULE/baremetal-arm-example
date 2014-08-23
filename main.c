#include "uemd_early.h"

// ~ 1 or 2 seconds on 300 MHz
#define DELAY (30*10000)
#ifndef bool
#define bool int
#define false 0
#define true 1
#endif

#define PHYS_IM0              0x00100000
#define LOAD_ADDR             (void*)0x47000000
#define NAND_OFFSET           0x20000
#define NAND_SIZE             0x80000


struct uemd_otp g_otp;

/* rom stuff */
void (*nand_init)(struct uemd_nand_state *state);
int (*nand_read)(struct uemd_nand_state *state, int start, void *dest, int words_len);
int (*otp_create)(struct uemd_otp *otp);


void __attribute__((naked,noreturn)) uemd_longjump(unsigned long* image_address)
{
    asm volatile("bx r0");
}


/* NAND layout */
/* 0x0: checksum */
/* 0x10: we are here */
/* 0x40000: something we load */


void main( void )
{
	volatile int i,k;
	int* data = (char*) LOAD_ADDR;
	/* hook rom functions */
	nand_init = 0x000007e0;
	nand_read = 0x000009f4;
	otp_create = 0x00001978;
	otp_create(&g_otp);
	uemd_em_init();
	struct uemd_nand_state nand;
	nand_init(&nand);
        for (i = 0; i < 8; i++ ) {
		nand_read(&nand, NAND_OFFSET, LOAD_ADDR, NAND_SIZE);
		if (*data == 1879)
			uemd_longjump(LOAD_ADDR);
        }
}
