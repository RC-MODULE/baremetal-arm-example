/*
 * (C) Copyright 2010
 * Sergey Mironov <ierton@gmail.com>
 *
 * See file CREDITS for list of people who contributed to this
 * project.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of
 * the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston,
 * MA 02111-1307 USA
 */
#ifndef UEMD_EARLY_H
#define UEMD_EARLY_H

#ifndef bool
#define bool int
#define false 0
#define true 1
#endif

struct uemd_nand_state {
    int size;
    int oobsize;
    int pagesize;
    int erasesize;
};

enum BootSource
{
    BOOT_FROM_RAM = 0,
	BOOT_FROM_SPI,
	BOOT_FROM_NAND
};

typedef struct KeyS{
    char data[16];
} Key;

struct uemd_otp
{
	bool jtag_stop; // need to stop in JTAG wait point

	enum BootSource source;

	bool image_signed; // image is signed
	unsigned int words_length; // image length

	unsigned int spi_speed;

	Key *pSignature; // if not null, image is signed in OTP, otherwise - in image
	Key *pKey; // if not null, image is encrypted with key
};

extern struct uemd_otp g_otp;

#define PHYS_EM0              0x40000000
#define PHYS_EM0_SIZE         _PHYS_EM0_SIZE
#define PHYS_EM1              0xC0000000
#define PHYS_EM1_SIZE         0x08000000

void uemd_em_init(void);
#endif

