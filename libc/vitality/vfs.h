#ifndef VFS_H
#define VFS_H

typedef struct file {
    char* name;
    char* data;
} file_t;

///
/// Partition Table Entry
///
typedef struct pte {
    char attributes;
    char chs[3];
    char partitiontype;
    char chslps[3];
    int lba;
    int sectors;
} pte_t;

///
/// Master Boot Record
///
typedef struct mbr {
    char bootstrap[440];
    char signature[4];
    pte_t firstpte;
    pte_t secondpte;
    pte_t thirdpte;
    pte_t fourthpte;
    char bssignature[2];
} mbr_t;

///
/// Post Mortem File Load
///
typedef struct pmfl {
    byte* ptr;
    unsigned int len;
} pmfl_t;

///
/// File Request CONFirmation
///
typedef struct frconf {
    bool found;
    void* ptr;
} frconf_t;

#endif