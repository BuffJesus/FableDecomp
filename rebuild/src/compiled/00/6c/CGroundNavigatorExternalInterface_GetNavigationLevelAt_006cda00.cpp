// Forwarder to a member of the Creature-object pointer at this+0x4. VC7.1 tail-calls
// a void member->void member forward, so retail is `mov ecx,[ecx+d]; jmp rel32`.
#include "engine/CGroundNavigatorExternalInterface.h"  // retyped onto the PDB layout; byte parity re-verified
#pragma pack(push,1)
struct CThingCreatureBase { void Run(); };
struct CGroundNavigatorExternalInterface_Methods : CGroundNavigatorExternalInterface {
    void Run();
};
#pragma pack(pop)
void CGroundNavigatorExternalInterface_Methods::Run() { this->Creature->Run(); }