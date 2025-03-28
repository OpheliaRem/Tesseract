OUTPUT_FORMAT(elf32-i386)
ENTRY(start)

PHDRS {
    text PT_LOAD FLAGS(5);  /* 5 = R | X */
    data PT_LOAD FLAGS(6);  /* 6 = R | W */
}

SECTIONS {
    . = 0x100000;

    .text : { *(.text) } : text
    .data : { *(.data) } : data
    .bss : { *(.bss) } : data
}