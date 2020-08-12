#ifndef VEP_H
#define VEP_H
#include "../types.h"
#include "../string.h"

///
/// This VEP will define a GDT
///
#define VEP_MAKESGDT 0b00000001
///
/// This VEP will define an IDT
/// 
#define VEP_MAKESIDT 0b00000010
///
/// This VEP will be a libraryVEP
///
#define VEP_LIBRARYV 0b00000100

///
/// Header for a VEP file
/// VEP stands for Vitality Executable Program
///
typedef struct vepheader {
    ///
    /// VEP signature, should be 'VP'
    ///
    char signature[2];
    ///
    /// VEP flags
    /// 
    char flags;
    ///
    /// 255 character array for name
    ///
    char name[255];
    ///
    /// Where the VEP should be loaded
    ///
    uint32_t origin;
    ///
    /// VEP size
    ///
    uint32_t vepsize;
} vepheader_t;

///
/// Standard Generated VEP file
///
typedef struct vepfile {
    ///
    /// VEP header
    ///
    vepheader_t vepheader;
    ///
    /// VEP Data
    ///
    char vepdata[65535];
} vepfile_t;

///
/// Loads a VEP with a VEPHeader
///
int loadvep(vepheader_t vep, char* data);

///
/// Loads a VEP with a VEPFile
///
int loadvepf(vepfile_t vep);
#endif