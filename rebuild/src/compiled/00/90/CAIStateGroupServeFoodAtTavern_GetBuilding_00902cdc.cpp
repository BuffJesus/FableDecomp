// Retail 0x00902CDC. Follow the state-group owner chain, then resolve the
// embedded building pointer at +0x170 through CBaseIntelligentPointer::GetPItem.
#include "engine/CBaseObject.h"
struct CThingBuilding : CBaseObject {};

// CBaseIntelligentPointer::GetPItem (retail 0x00A01B50).
CBaseObject* __fastcall ResolveIntelligentPointer(void* pPointer);

struct CStateGroupOwner {
    char m_leading[0x18];
    char* m_pIntelligentPointers;
};

struct CAIStateGroup_ServeFoodAtTavern {
    void* __vftable;
    CStateGroupOwner* m_pOwner;
    CThingBuilding* GetBuilding();
};

CThingBuilding* CAIStateGroup_ServeFoodAtTavern::GetBuilding() {
    char* intelligentPointers = m_pOwner->m_pIntelligentPointers;
    return static_cast<CThingBuilding*>(
        ResolveIntelligentPointer(intelligentPointers + 0x170));
}