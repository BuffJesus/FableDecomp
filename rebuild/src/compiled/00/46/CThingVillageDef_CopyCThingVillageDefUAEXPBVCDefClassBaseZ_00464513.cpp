#pragma optimize("s",on)
// CThingVillageDef::Copy  @ 0x00464513
// Base has a non-virtual Copy(const CDefClassBase*) at 0x463950 (CThingBaseDef::CopyBase);
// derived overrides the virtual Copy and additionally copies its own 14-byte POD at +0x70.

class CDefClassBase {
public:
    virtual void Copy(const CDefClassBase* src) = 0;  // vptr at offset 0
};

class CThingBaseDef : public CDefClassBase {
public:
    void CopyBase(const CDefClassBase* src);   // non-virtual base copy @ 0x463950
    // vptr(4) + this pad brings total base size to 0x70
    char pad[0x70 - 4];
};

struct VillageData {
    unsigned char bytes[14];
};

class CThingVillageDef : public CThingBaseDef {
public:
    VillageData m_data;                    // at +0x70
    virtual void Copy(const CDefClassBase* src);
};

void CThingVillageDef::Copy(const CDefClassBase* src)
{
    const CThingVillageDef* s = static_cast<const CThingVillageDef*>(src);
    CThingBaseDef::CopyBase(src);
    m_data = s->m_data;
}