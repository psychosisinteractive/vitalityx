#include "page.h"
#include "types.h"
#include "ext/debug.h"
uint32_t page_directory[1024] 
#ifndef __INTELLISENSE__ 
    __attribute__((aligned(4096)));
#else
    ;
#endif

void setup_page() {
    BochsConsolePrintString("page being setup\n");
    //set each entry to not present
    int i;
    for(i = 0; i < 1024; i++)
    {
        // This sets the following flags to the pages:
        //   Supervisor: Only kernel-mode can access them
        //   Write Enabled: It can be both read from and written to
        //   Not Present: The page table is not present
        page_directory[i] = 0x00000002;
    }
    loadPageDirectory(page_directory);
    enablePaging();
    BochsConsolePrintString("pages setup\n");
}

void create_page_table(unsigned int i, unsigned int id) {
    BochsConsolePrintString("setting up page table\n");
    uint32_t page_table[1024]
    #ifndef __INTELLISENSE__ 
        __attribute__((aligned(4096)));
    #else
        ;
    #endif
    //we will fill all 1024 entries in the table, mapping 4 megabytes
    for(i = 0; i < 1024; i++)
    {
        // As the address is page aligned, it will always leave 12 bits zeroed.
        // Those bits are used by the attributes ;)
        page_table[i] = (i * 0x1000) | 3; // attributes: supervisor level, read/write, present.
    }
    page_directory[id] = ((unsigned int)page_table) | 3;
    BochsConsolePrintString("page table setup\n");
}