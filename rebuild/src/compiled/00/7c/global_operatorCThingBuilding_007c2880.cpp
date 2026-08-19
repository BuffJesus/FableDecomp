struct CThingBuilding;

struct CIntelligentPointer {
    CThingBuilding* p;
};

// callee at 0xa01b50: CIntelligentPointer<CThingBuilding>::Get / dereference
extern "C" CThingBuilding* __fastcall IP_Get(CIntelligentPointer* self);

bool __fastcall thingbuilding_ip_equal(CIntelligentPointer* a, CIntelligentPointer* b)
{
    return IP_Get(a) == IP_Get(b);
}