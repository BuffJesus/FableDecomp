#include "engine/CResource.h"  // retyped onto the PDB layout; byte parity re-verified
struct CResourceList { char pad[0x2c]; int f2c; char pad3[0x34-0x30]; int f34; };
struct CResource_Methods : CResource {
    void M();
};
void CResource_Methods::M(){ CResourceList* p=this->ResourceList; if(p) this->LastUsedFrame = p->f2c - p->f34; }