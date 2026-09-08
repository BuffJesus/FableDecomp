// CManagedTextureData_Methods::IsBusy  @ retail 0x00a69810
// __fastcall bool IsBusy(CManagedTextureData* this)
//
// Reads a "BusyFlag" flag at +0x4a; if set, re-queries the underlying
// Texture_Data object at +0x20 (a __fastcall query at 0x9f9d50) and
// caches the result back into +0x4a; returns the flag.

#include "engine/CManagedTextureData.h"  // retyped onto the PDB layout; byte parity re-verified
struct CTexture
{
    bool CheckBusy();   // __fastcall @ 0x9f9d50
};

struct CManagedTextureData_Methods : CManagedTextureData {
    // +0x4a

    bool IsBusy();
};

bool CManagedTextureData_Methods::IsBusy()
{
    if (this->BusyFlag)
    {
        this->BusyFlag = this->Texture_Data->CheckBusy();
    }
    return this->BusyFlag;
}