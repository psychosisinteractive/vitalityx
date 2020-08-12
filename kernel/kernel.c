#include "../libc/vitality/tty.h"
#include "../libc/interrupt.h"
#include "../libc/page.h"
#include "../libc/ext/debug.h"
///
/// The kernels C entry point
///
int kernel() {
    BochsConsolePrintString("VitalityX started\n");
    setup_page(); // enable paging
    tty_pputstring("VitalityX");
    idt_install();
    for(;;) {

    }
    return 0;
}