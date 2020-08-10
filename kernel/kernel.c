#include "../libc/vitality/tty.h"
#include "../libc/interrupt.h"
///
/// The kernels C entry point
///
int kernel() {
    idt_install();
    tty_vgaputstring("Hello World");
    for(;;) {

    }
    return 0;
}