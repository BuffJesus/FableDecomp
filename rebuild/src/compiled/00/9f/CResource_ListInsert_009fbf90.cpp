#include "engine/CResource.h"  // retyped onto the PDB layout; byte parity re-verified
struct CResource_Methods : CResource {
    // +0x10
    void ListInsert(CResource* p1, CResource* p2);
};

void CResource_Methods::ListInsert(CResource* p1, CResource* p2)
{
    this->PrevResource = p1;
    this->NextResource = p2;
    p1->NextResource = this;
    this->NextResource->PrevResource = this;
}