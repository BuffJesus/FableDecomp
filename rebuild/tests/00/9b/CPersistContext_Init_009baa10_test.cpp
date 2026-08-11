#include <cstdio>

class CCharString;
class CDefinitionManager;

class CPersistContext {
public:
    void Init(CCharString *name, CDefinitionManager *defMgr);
};

struct CPersistContextLayout {
    CDefinitionManager *defMgr;
    char pad1[0x11];
    char flag15;
    char pad2[0x02];
    int  field18;
    char pad3[0x04];
    CCharString *name;
};

void CPersistContext::Init(CCharString *name, CDefinitionManager *defMgr)
{
    CPersistContextLayout *self = reinterpret_cast<CPersistContextLayout *>(this);
    self->field18 = 1;
    self->name = name;
    self->defMgr = defMgr;
    self->flag15 = 0;
}

int main()
{
    unsigned char buf[0x40];
    for (int i = 0; i < 0x40; ++i) buf[i] = 0xAB;

    CCharString *fakeName = reinterpret_cast<CCharString *>(0x11223344);
    CDefinitionManager *fakeMgr = reinterpret_cast<CDefinitionManager *>(0x55667788);

    CPersistContext *ctx = reinterpret_cast<CPersistContext *>(buf);
    ctx->Init(fakeName, fakeMgr);

    CPersistContextLayout *L = reinterpret_cast<CPersistContextLayout *>(buf);
    bool ok = true;
    if (L->field18 != 1) ok = false;
    if (L->name != fakeName) ok = false;
    if (L->defMgr != fakeMgr) ok = false;
    if (L->flag15 != 0) ok = false;

    if (ok) printf("PERSISTINIT_OK\n");
    else    printf("PERSISTINIT_FAIL f18=%d name=%p mgr=%p f15=%d\n",
                   L->field18, (void*)L->name, (void*)L->defMgr, (int)L->flag15);
    return ok ? 0 : 1;
}