#pragma once
#include "rebuild_abi.h"
// PDB CSystemFontCharacter: one pointer to bitmap elements.
class CSystemFontCharacter {
public:
    bool Initialise(char* elements); // 00A6C140
    bool IsElementSolid(unsigned long x,unsigned long y); // 00A6C150
    char* Elements;
};
FABLE_STATIC_ASSERT(sizeof(CSystemFontCharacter)==4);
// Descriptive names; native storage addresses confirmed by Initialise A6C170.
extern CSystemFontCharacter FableSystemFontCharacters_013d28f0[256];
extern bool FableSystemFontInitialized_013d2cf0;
extern char FableSystemFontBlank_013d2cf4[36];
extern char FableSystemFontGlyphs_01397c60[60][36];
