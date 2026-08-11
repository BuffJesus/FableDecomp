// CPersistContext::Init  0x009baa10  __fastcall(void)
class CCharString;
class CDefinitionManager;

class CPersistContext {
public:
    void Init(CCharString *name, CDefinitionManager *defMgr);
};

struct CPersistContextLayout {
    CDefinitionManager *defMgr;   // +0x00
    char pad1[0x11];              // +0x04 .. +0x14
    char flag15;                  // +0x15
    char pad2[0x02];              // +0x16 .. +0x17
    int  field18;                 // +0x18
    char pad3[0x04];              // +0x1C
    CCharString *name;            // +0x20
};

void CPersistContext::Init(CCharString *name, CDefinitionManager *defMgr)
{
    CPersistContextLayout *self = reinterpret_cast<CPersistContextLayout *>(this);
    self->field18 = 1;
    self->name = name;
    self->defMgr = defMgr;
    self->flag15 = 0;
}