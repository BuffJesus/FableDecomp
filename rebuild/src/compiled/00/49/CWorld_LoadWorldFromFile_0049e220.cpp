// CWorld::LoadWorldFromFile  @ 0x0049e220
#include "engine/CWorld.h"  // retyped onto the PDB layout; byte parity re-verified

// CAFile vtable model (header gives no typed CAFile): slot 5 = Finish, slot 7 = Read
struct CAFile
{
    virtual void V0();
    virtual void V1();
    virtual void V2();
    virtual void V3();
    virtual void V4();
    virtual void Finish(long value);
    virtual void V6();
    virtual long Read();
};

// CWorld.h only forward-declares CWorldMap; local vtable model, slot 3 = Load
struct CWorldMap
{
    virtual void V0();
    virtual void V1();
    virtual void V2();
    virtual void Load(CAFile* file, long value);
};

// CWorld's own vtable reached through the header's plain `void* __vftable`; slot 1 = FinishLoad
struct CWorldVtbl
{
    void* Slot0;
    void (__fastcall *FinishLoad)(CWorld* self);
};

struct CWorld_Methods : CWorld
{
    void LoadWorldFromFile(CAFile* file, long value);
};

extern float CWorldLoadTimer();

void CWorld_Methods::LoadWorldFromFile(CAFile* file, long value)
{
    (void)CWorldLoadTimer();
    long result = file->Read();
    this->PWorldMap_ptr->Load(file, value);
    file->Finish(result);
    ((CWorldVtbl*)this->__vftable)->FinishLoad(this);
    (void)CWorldLoadTimer();
}
