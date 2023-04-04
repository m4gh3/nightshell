PHDRS
{
	loadable PT_LOAD;
	metadata PT_LOAD; /* will not be loaded */
}

SECTIONS
{
  . = 0x0;
  .text : { *(.text) } : loadable
  .got  : { *(.got) } : loadable
  .data : { *(.data) } : loadable
  .rel.plt : { *(.rel.plt) } : loadable
  .dynsym : { *(.dynsym) } : loadable
  .dynstr : { *(.dynstr) } : loadable
  .bss : { *(.bss) } : loadable
}
