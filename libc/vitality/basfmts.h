#ifndef BASFMTS_H
#define BASFMTS_H
#include "../types.h"

///
/// PSF Magic
///
#define PSF_FONT_MAGIC 0x864ab572

///
/// PC Screen Font struct
///
typedef struct psf {
    ///
    /// PSF Magic, should be PSF_FONT_MAGIC
    ///
    uint32_t magic;         
    ///
    /// PSF Version
    ///
    uint32_t version;      
    ///
    /// Size of the HEader
    ///
    uint32_t headersize;
    ///
    /// Flags
    ///   
    uint32_t flags; 
    ///
    /// Number of Glyphs
    ///        
    uint32_t numglyph;      
    ///
    /// Bytes per Glyph
    ///
    uint32_t bytesperglyph;
    ///
    /// Height in Pixels
    ///
    uint32_t height;       
    ///
    /// Width in Pixels
    ///
    uint32_t width;        
} psf_t;

#endif