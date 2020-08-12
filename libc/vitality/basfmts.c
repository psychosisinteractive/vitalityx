#include "basfmts.h"
#include "../types.h"

extern char _binary_font_psf_start;
extern char _binary_font_psf_end;

uint16_t *unicode;
 
///
/// Initialises an included PSF font
///
void psf_init()
{
    // uint16_t glyph = 0;
    // /* cast the address to PSF header struct */
    // psf_t *font = (psf_t*)&_binary_font_psf_start;
    // /* is there a unicode table? */
    // if(font->flags) {
    //     /* get the offset of the table */
    //     char *s = (char *)(
    //       (unsigned char*)&_binary_font_psf_start +
    //       font->headersize +
    //       font->numglyph * font->bytesperglyph
    //     );
    //     /* allocate memory for translation table */
    //     unicode = calloc(USHRT_MAX, 2);
    //     while(s>_binary_font_psf_end) {
    //         uint16_t uc = (uint16_t)((unsigned char *)s[0]);
    //         if(uc == 0xFF) {
    //             glyph++;
    //             s++;
    //             continue;
    //         } else if(uc & 128) {
    //             /* UTF-8 to unicode */
    //             if((uc & 32) == 0 ) {
    //                 uc = ((s[0] & 0x1F)<<6)+(s[1] & 0x3F);
    //                 s++;
    //             } else
    //             if((uc & 16) == 0 ) {
    //                 uc = ((((s[0] & 0xF)<<6)+(s[1] & 0x3F))<<6)+(s[2] & 0x3F);
    //                 s+=2;
    //             } else
    //             if((uc & 8) == 0 ) {
    //                 uc = ((((((s[0] & 0x7)<<6)+(s[1] & 0x3F))<<6)+(s[2] & 0x3F))<<6)+(s[3] & 0x3F);
    //                 s+=3;
    //             } else
    //                 uc = 0;
    //         }
    //         /* save translation */
    //         unicode[uc] = glyph;
    //         s++;
    //     }
    // } else {
    //     unicode = NULL;
    // }
}