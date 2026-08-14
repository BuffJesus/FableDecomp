// Byte-exact reconstruction of operator==(CIntelligentPointer<CThingBuilding>&, CIntelligentPointer<CThingBuilding>&) @ 0x0065d673
struct CThingBuilding;

struct CIntelligentPointer_CThingBuilding_ {
    CThingBuilding* Get();
};

// retail 0xa01b50 = CIntelligentPointer<CThingBuilding>::Get, this-in-ecx.
// modeled as __fastcall free fn (self in ecx). extern so a real call is emitted.
extern CThingBuilding* __fastcall CIP_Get(CIntelligentPointer_CThingBuilding_* self);

bool __fastcall operator_eq_CThingBuilding_(CIntelligentPointer_CThingBuilding_* lhs,
                                            CIntelligentPointer_CThingBuilding_* rhs)
{
    return CIP_Get(lhs) == CIP_Get(rhs);
}