// CIntelligentPointer<CThingCreatureBase>::operator=
// retail 0x00668b80

class CThingCreatureBase;

class CIntelligentPointer
{
public:
    CIntelligentPointer &operator=(CIntelligentPointer &other);
    // 0xa01b50: read the referenced object out of 'other'
    CThingCreatureBase *GetObject();
    // 0xa01b90: assign the referenced object into 'this'
    void Assign(CThingCreatureBase *obj);

    void *m_data;
};

CIntelligentPointer &CIntelligentPointer::operator=(CIntelligentPointer &other)
{
    Assign(other.GetObject());
    return *this;
}