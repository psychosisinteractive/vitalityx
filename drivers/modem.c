#include "modem.h"
#include "serial.h"
#include "../libc/types.h"

bool commandmode = true;
bool modemstate = false;

bool acknowledge() {
    if(read_serial() == 'O') {
        if(read_serial() == 'K') {
            return true;
        }
        return false;
    }
    return false;
}

void alternate_mode() {
    write_serial_string("+++");
    if(serial_received()) {
        commandmode = acknowledge();
    }
}

void modem_dial(char* ph) {
    write_serial_string("ATDT");
    write_serial_string(ph);
}

void modem_connect() {
    if(serial_received()) {
        bool ds = false;
        if(read_serial() == 'R') {
            if(read_serial() == 'I') {
                if(read_serial() == 'N') {
                    if(read_serial() == 'G') {
                        ds = true;
                        read_serial();
                    }
                }
            }
        }
        write_serial_string("ATA");
    }
}

void getmodem() {
    write_serial_string("AT");
    if(serial_received()) {
        modemstate = acknowledge();
    }
}