#include "../libc/vitality/tty.h"
#include "../libc/descriptortables.h"
#include "../libc/page.h"
#include "../libc/ext/debug.h"
#include "../drivers/vga.h"
#include "../libc/ext/task.h"
#include "../libc/vitality/mem.h"
#include "../libc/vitality/vlib.h"
///
/// The kernels C entry point
///
vlib_entry_t vlibe[255];
vlibdesriptor_t vlibd;
int kernel() {
    setup_page(); // enable paging
    dt_install();
    setupmalloc();
    loadvlib(&vlibd);
    setfont(0x7e00);
    initTasking();
    BochsConsolePrintString("VitalityX started\n");
    tty_pputstring("VitalityX");
    mempoke(0,5,'x');
    asm volatile("int $0x3f");
    for(;;) {
        yield();
    }
    return 0;
}