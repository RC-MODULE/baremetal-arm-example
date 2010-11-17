OUTPUT_ARCH(ARM)
ENTRY(_start)
SECTIONS
{
    . = 0;
	. = ALIGN(4);
    .text : {
		start.o (.text)
		*(.text)
    }

    .rodata	: {
		*(.rodata.*)
    }

    .= 0x140000;
    .bss : {
		*(.bss)
    }
}
