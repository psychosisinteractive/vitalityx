#ifndef FAT12_H
#define FAT12_H
#include "../libc/types.h"

#define FAT_READ_ONLY 0x01
#define FAT_HIDDEN 0x2
#define FAT_SYSTEM 0x4
#define FAT_VOL_ID 0x8
#define FAT_DIRECTORY 0x10
#define FAT_ARCHIVE 0x20
#define FAT_LFN FAT_READ_ONLY|FAT_HIDDEN|FAT_SYSTEM|FAT_VOL_ID

/**
 * Fat BIOS Parameter Block
 */
typedef struct fat_bpb
{
    unsigned char BPBCode[3]; // BPB code
    unsigned char OEM[8]; // OEM identifier
    unsigned short bytespersector; // Bytes Per Sector
    unsigned char sectorspercluster; // Sectors Per Cluster
    unsigned short reservedsectors; // Reserved Sectors
    unsigned char fats; // FATS on Drive
    unsigned short directories; // Directories
    unsigned short sectorslogical; // Logical Sectors
    unsigned char mdt; // Media Descriptor Type
    unsigned short sectorsperfat; // Sectors Per FAT
    unsigned short sectorspertrack; // Sectors Per Track
    unsigned short headsorsides; // Heads or Sides on Storage Media
    unsigned short hiddensectors; // Hidden Sectors
    unsigned short largesectorcount; // large sectors count
}__attribute__((packed)) fat_bpb_t;

/**
 * Fat Extended Boot Record 16 bit
 */
struct fat_ebr16
{
    unsigned char drivenum; // drive number
    unsigned char ntflags; // nt flags
    unsigned char signature; // signature
    unsigned char serial[4]; // serial code
    unsigned char label[11]; // label
    unsigned char sistring[8]; // string
    unsigned char bootcode[448]; // boot code, skipped over because we already boot on a fda
    unsigned short bpartsign; // bpart
}__attribute__((packed)) fat_ebr16_t;

/**
 * FAT directory
 */
struct fat_directory 
{
    unsigned char filename[11]; // 8.3 filename (last 3 chars are the file extension)
    unsigned char attributes; // file attributes
    unsigned char ntexclusive; // nt exclusive data
    unsigned char creationtime; // creation time
    unsigned short time; // time
    unsigned char date; // date
    unsigned char ladate; // last access dated
    unsigned short exclusive32h; // exclusive 32 bit data
    unsigned char lamtime; // last modified time
    unsigned char lamdate; // last modified date
    unsigned short fclusternumber; // cluster number
    unsigned int size; // size
}__attribute__((packed)) fat_directory_t;

/** Info of the FAT, supplied by driver
 */
struct fat_drvinfo 
{
    unsigned int fat_size; // size of fat
    unsigned int total_sectors; // total sectors in vol
    unsigned int fat_size_sectors; // fat size in sectors
    unsigned int root_dir_sectors; // size of root directory 
    unsigned int first_data_sector; // first data sector
    unsigned int first_fat_setor; // first fat sector
    unsigned int data_sectors; // total number of data sectors
    unsigned int total_clusters; // total number of clusters
    struct fat_ebr16* ebr; // the ebr
    struct fat_bpb* bpb; // the bpb
} typedef fat_drvinfo_t;

/** FAT type enum, all types of FAT
 */
enum fattype
{
    /** FAT12 */
    FAT12, 
    /**< FAT16 */
    FAT16, 
    /**< FAT32 */
    FAT32, 
    /**< Unknown FAT type, ex ExFAT or a non-FAT filesystem */
    UNKNOWN, 
} typedef fattype_t;

/* Gets the type of a FAT in enum fattype_t
 * @param fat The FAT driver info.
 */
fattype_t fat_gettype(struct fat_drvinfo fat) {
    int total_clusters = fat.total_clusters;
    if(total_clusters < 4085) {
        return FAT12;
    } else if(total_clusters < 65525) {
        return FAT16;
    } else if(total_clusters < 268435445) {
        return FAT32;
    } else {
        return UNKNOWN;
    }
}

/** fat_getebr16 returns a FAT Extended Boot Record.
 *@param bpb The FAT Bios Partition Block
 */
struct fat_ebr16* fat_getebr16(struct fat_bpb bpb) {
    return (struct fat_ebr16*)((char*)&bpb + sizeof(bpb));
}

/** fat_getfdir is under construction.
 */
void fat_getfdir(struct fat_drvinfo fat) {
    int first_root_dir_sector = fat.first_data_sector - fat.root_dir_sectors;

}

/** fat_getinfo returns a fat_drvinfo.
 *@param bpb The BIOS Partition Table.
 */
struct fat_drvinfo fat_getinfo(struct fat_bpb bpb) {
    struct fat_drvinfo info;
    info.total_sectors = bpb.sectorslogical;
    info.fat_size = bpb.sectorsperfat;
    info.root_dir_sectors = bpb.directories;
    info.first_data_sector = bpb.reservedsectors;
    info.data_sectors = bpb.sectorslogical - (bpb.reservedsectors + (bpb.fats * bpb.sectorsperfat));
    info.total_clusters = info.data_sectors / bpb.sectorspercluster;
    info.bpb = &bpb;
    *bpb.OEM = "VITALITY";
    info.ebr = fat_getebr16(bpb);
    return info;
}

#endif