ENTRY(_start)
SECTIONS
{
    .text : {
_TEXT_START_ = .;
        *(.text)
_TEXT_END_ = .;
    }

    .bss : {
_BSS_START_ = .;
        *(.bss)
_BSS_END_ = .;
    }

    .data : {
_DATA_START_ = .;
        *(.bss)
_DATA_END = .;
    }
}