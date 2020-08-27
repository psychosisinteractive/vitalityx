ENTRY(_start)
SECTIONS
{
    .text : ALIGN(512) {
_TEXT_START_ = .;
        *(.text)
_TEXT_END_ = .;
    }

    .data : ALIGN(512) {
_DATA_START_ = .;
        *(.bss)
_DATA_END = .;
    }

    .virt : ALIGN(512) {
_VIRTUAL_8086_START = .;
        *(.virt)
_VIRTUAL_8086_END = .;
    }

    .bss : ALIGN(512) {
_BSS_START_ = .;
        *(.bss)
_BSS_END_ = .;
    }
}