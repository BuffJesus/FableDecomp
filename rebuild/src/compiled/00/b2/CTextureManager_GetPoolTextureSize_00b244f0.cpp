#include "engine/CTextureManager.h"  // retyped onto the PDB layout; byte parity re-verified
struct CTextureManager_Methods : CTextureManager {
    unsigned long GetPoolTextureSize(unsigned long i);
};
unsigned long CTextureManager_Methods::GetPoolTextureSize(unsigned long i){ return *(unsigned long*)((char*)this + i*4 + 0x120); }