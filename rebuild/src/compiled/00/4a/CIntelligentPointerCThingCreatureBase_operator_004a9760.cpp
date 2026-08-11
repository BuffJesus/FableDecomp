// CIntelligentPointer<CThingCreatureBase>::operator=
// Smart-pointer copy assignment: this->Assign( rhs->GetTarget() ); return this;

class CThingCreatureBase;

class CIntelligentPointer
{
public:
    // 0xa01b50 : returns the held target pointer (getter used to source rhs)
    CThingCreatureBase* GetTarget();
    // 0xa01b90 : assigns/refs a target into this pointer
    void Assign(CThingCreatureBase* p);

    CIntelligentPointer* operator=(CIntelligentPointer* rhs);

    CThingCreatureBase* m_ptr;
};

// Out-of-line so the compiler emits the body into the .obj.
CIntelligentPointer* CIntelligentPointer::operator=(CIntelligentPointer* rhs)
{
    this->Assign(rhs->GetTarget());
    return this;
}