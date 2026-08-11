// CIntelligentPointer<CThingCreatureBase>::operator= (retail 0x0050f3d0)
class CThingCreatureBase;

class CIntelligentPointer_CThingCreatureBase
{
public:
    // out-of-line helpers (bodies live elsewhere in the binary):
    //   GetRaw() -> raw pointer/handle (retail 0x00a01b50)
    //   SetRaw(x)                       (retail 0x00a01b90)
    CThingCreatureBase* GetRaw();
    void SetRaw(CThingCreatureBase* p);

    CIntelligentPointer_CThingCreatureBase* operator=(
        CIntelligentPointer_CThingCreatureBase* rhs);
};

CIntelligentPointer_CThingCreatureBase*
CIntelligentPointer_CThingCreatureBase::operator=(
    CIntelligentPointer_CThingCreatureBase* rhs)
{
    this->SetRaw(rhs->GetRaw());
    return this;
}