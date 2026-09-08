#include <stdio.h>
static void* g_pointerThis = 0;
struct CBaseObject {};
struct CThingBuilding : CBaseObject {};
static CThingBuilding g_building;
CBaseObject* __fastcall ResolveIntelligentPointer(void* value) {
    g_pointerThis = value;
    return &g_building;
}
struct CStateGroupOwner { char m_leading[0x18]; char* m_pIntelligentPointers; };
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
int main() {
    char pointerStorage[0x200];
    CStateGroupOwner owner;
    owner.m_pIntelligentPointers = pointerStorage;
    CAIStateGroup_ServeFoodAtTavern state;
    state.m_pOwner = &owner;
    if (state.GetBuilding() != &g_building) return 1;
    if (g_pointerThis != pointerStorage + 0x170) return 2;
    printf("SERVE_FOOD_TAVERN_BUILDING PASS\n");
    return 0;
}