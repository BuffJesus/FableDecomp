// CIntelligentPointer<CThingCreatureBase>::operator=  @ 0x00c16950
class CThingCreatureBase;

class CIntelligentPointer
{
public:
    void* m_ptr;
    // 0x00c16800 : thiscall on the source pointer, returns a CThingCreatureBase* in eax
    CThingCreatureBase* GetTarget();
    // 0x00c16820 : thiscall on the destination pointer, takes the resolved target
    void SetTarget(CThingCreatureBase*);

    CIntelligentPointer* operator=(CIntelligentPointer& rhs);
};

CIntelligentPointer* CIntelligentPointer::operator=(CIntelligentPointer& rhs)
{
    this->SetTarget(rhs.GetTarget());
    return this;
}