#include "../libc/vitality/tty.h"
#include "../libc/descriptortables.h"
#include "../libc/page.h"
#include "../libc/ext/debug.h"
#include "../drivers/vga.h"
#include "../libc/ext/task.h"
///
/// The kernels C entry point
///
int kernel() {
    dt_install();
    setup_page(); // enable paging
    setfont(0x7e00);
    initTasking();
    BochsConsolePrintString("VitalityX started\n");
    tty_pputstring("VitalityX");
    for(;;) {
        yield();
    }
    return 0;
}