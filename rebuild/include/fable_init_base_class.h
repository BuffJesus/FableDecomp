#pragma once

#include "fable_base_class.h"

// FableWin.pdb / retail RTTI: CInitBaseClass : CBaseClass, 8 bytes (vtable, Valid).
// Retail vtable 0x0129A7CC: scalar deleting destructor 0x0099A490, then
// Validate 0x0099A340. The out-of-line bodies (0x0099A310..0x0099A490) share
// one translation unit with CBaseClass, so they inline each other there but are
// called everywhere else.
class CInitBaseClass : public CBaseClass
{
public:
    CInitBaseClass();
    virtual ~CInitBaseClass();

    bool IsValid() const;
    virtual void Validate() const;

protected:
    bool Initialise();
    void Uninitialise();

    bool Valid;
};

// Unnamed retail counters: the constructor bumps 0x013BCA18 and Uninitialise
// drops 0x013BCA14 when a valid object is torn down. The PDB gives no names.
extern long g_CInitBaseClassConstructed_013BCA18;
extern long g_CInitBaseClassValid_013BCA14;

FABLE_STATIC_ASSERT(sizeof(CInitBaseClass) == 0x08);
