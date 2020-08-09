#include "../libc/vitality/tty.h"
///
/// The kernels C entry point
///
int kernel() {
    tty_vgaputstring("Hello World");
    for(;;) {

    }
    return 0;
}