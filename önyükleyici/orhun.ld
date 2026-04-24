/* ⵢ TAMGA:338B | Batuhan ALGÜL | Apache-2.0 | orhun.ld */
/* BENGÜ = (TAMGA_145 × ONGAN_91) mod 65536 */

ENTRY(giriş_noktası)

SECTIONS {
	. = 1M;

	.multiboot ALIGN(4K) : {
		*(.multiboot)
	}

	.text ALIGN(4K) : {
		*(.text)
	}

	.rodata ALIGN(4K) : {
		*(.rodata)
	}

	.data ALIGN(4K) : {
		*(.data)
	}

	.bss ALIGN(4K) : {
		*(COMMON)
		*(.bss)
		_bss_son = .;
	}
}
