OUTPUT_FORMAT("binary")
ENTRY(start)
phys = 0x00100000;
SECTIONS
{
  .text phys : AT(phys) {
    code = .;
    *(.text)
    *(.gnu.linkonce.t.*)
    *(.gnu.linkonce.r.*)
    *(.rodata)
    . = ALIGN(4096);
  }
  .data : AT(phys + (data - code))
  {
    data = .;
    *(.data)
    start_ctors = .;
    *(.ctor*)
    end_ctors = .;
    start_dtors = .;
    *(.dtor*)
    end_dtors = .;
    *(.gnu.linkonce.d.*)
    . = ALIGN(4096);
  }
  .bss : AT(phys + (bss - code))
  {
    bss = .;
    *(.bss)
    *(.gnu.linkonce.b.*)
    . = ALIGN(4096);
  }
  end = .;
}

