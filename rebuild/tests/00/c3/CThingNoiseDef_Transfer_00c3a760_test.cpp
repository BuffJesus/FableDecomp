#include <cstdio>

struct CPersistContext;

static int g_base_called = 0;
static void* g_member_owner = 0;
static int g_member_flag = -1;

struct CThingNoiseDef_Base
{
    char base_bytes[0x18];
    void __fastcall Transfer(CPersistContext* ctx);
};

struct CThingNoiseMember
{
    void DoIt(void* owner, int flag);
};

struct CThingNoiseDef : public CThingNoiseDef_Base
{
    CThingNoiseMember m18;
    CThingNoiseDef* __fastcall Transfer(CPersistContext* ctx);
};

void __fastcall CThingNoiseDef_Base::Transfer(CPersistContext* ctx)
{
    g_base_called = 1;
}
void CThingNoiseMember::DoIt(void* owner, int flag)
{
    g_member_owner = owner;
    g_member_flag = flag;
}

CThingNoiseDef* __fastcall CThingNoiseDef::Transfer(CPersistContext* ctx)
{
    this->CThingNoiseDef_Base::Transfer(ctx);
    this->m18.DoIt(this, 1);
    return this;
}

int main()
{
    CThingNoiseDef obj;
    obj.Transfer((CPersistContext*)0);
    if (g_base_called == 1 && g_member_owner == (void*)&obj && g_member_flag == 1)
        printf("00c3a760_TEST PASS\n");
    else
        printf("FAIL b=%d owner=%p flag=%d\n", g_base_called, g_member_owner, g_member_flag);
    return 0;
}