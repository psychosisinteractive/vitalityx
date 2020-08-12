#ifndef PAGE_H
#define PAGE_H

void setup_page();
void create_page_table(unsigned int i, unsigned int id);
extern void loadPageDirectory(unsigned int*);
extern void enablePaging();

#endif