#pragma once
#include "fable_init_base_class.h"
// Ego_r CSystemFont has no data beyond the 8-byte CInitBaseClass.
class CSystemFontCharacter;
class CSystemFont : public CInitBaseClass {
public:
    CSystemFontCharacter* GetCharacter(unsigned char character); // 00A6C130
    bool Initialise(); // 00A6C170
    void Uninitialise(); // 00A6C120
};
FABLE_STATIC_ASSERT(sizeof(CSystemFont)==8);
