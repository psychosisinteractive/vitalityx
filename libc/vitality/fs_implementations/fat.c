#include "fat.h"
#include "../../types.h"

fat_descriptor_t* fat_gendescriptor(fat_BS_t* bs) {
    fat_descriptor_t descriptor;

    fat_extBS_32_t *fat_boot_ext_32 = NULL;
    fat_extBS_16_t *fat_boot_ext_16 = NULL;

    descriptor.total_sectors = (bs->total_sectors_16 == 0)? bs->total_sectors_32 : bs->total_sectors_16;
    descriptor.bs = &bs;
    if(descriptor.total_clusters < 4085) 
    {
        fat_boot_ext_16 = (fat_extBS_16_t*)bs->extended_section;
        descriptor.fat_type = FAT12;
    } 
    else if(descriptor.total_clusters < 65525) 
    {
        fat_boot_ext_16 = (fat_extBS_16_t*)bs->extended_section;
        descriptor.fat_type = FAT16;
    } 
    else if (descriptor.total_clusters < 268435445)
    {
        fat_boot_ext_32 = (fat_extBS_16_t*)bs->extended_section;
        descriptor.fat_type = FAT32;
    }
    else
    { 
        // idk
        descriptor.fat_type = EXFAT;
    }

    descriptor.fat_size = (bs->table_size_16 == 0)? fat_boot_ext_32->table_size_32 : bs->table_size_16;
}