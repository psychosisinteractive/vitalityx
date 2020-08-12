#include "../libc/vitality/tty.h"
#include "../libc/interrupt.h"
///
/// The kernels C entry point
///
int kernel() {
    tty_pputstring("Hello World");
    idt_install();
    for(;;) {

    }
    return 0;
}