#include <cstdio>

struct CPersistContext;

struct CThingNoiseDef_Base
{
    char base_bytes[0x0c];
    void __fastcall Transfer(CPersistContext* ctx);
};

struct CThingNoiseMember
{
    void DoIt(void* owner, int flag);
};

struct CThingNoiseDef : public CThingNoiseDef_Base
{
    CThingNoiseMember m0c;
    CThingNoiseDef* __fastcall Transfer(CPersistContext* ctx);
};

static int g_base_called = 0;
static void* g_base_this = 0;
static int g_doit_called = 0;
static void* g_doit_ecx = 0;
static void* g_doit_owner = 0;
static int g_doit_flag = 0;

void __fastcall CThingNoiseDef_Base::Transfer(CPersistContext* ctx)
{
    g_base_called++;
    g_base_this = this;
}

void CThingNoiseMember::DoIt(void* owner, int flag)
{
    g_doit_called++;
    g_doit_ecx = this;
    g_doit_owner = owner;
    g_doit_flag = flag;
}

CThingNoiseDef* __fastcall CThingNoiseDef::Transfer(CPersistContext* ctx)
{
    this->CThingNoiseDef_Base::Transfer(ctx);
    this->m0c.DoIt(this, 1);
    return this;
}

int main()
{
    CThingNoiseDef obj;
    CThingNoiseDef* r = obj.Transfer(0);
    bool ok = true;
    if (g_base_called != 1) ok = false;
    if (g_base_this != (void*)&obj) ok = false;
    if (g_doit_called != 1) ok = false;
    if (g_doit_ecx != (void*)&obj.m0c) ok = false;
    if (g_doit_owner != (void*)&obj) ok = false;
    if (g_doit_flag != 1) ok = false;
    if (r != &obj) ok = false;
    if (ok) printf("00c3a780_TEST PASS\n");
    else printf("FAIL b=%d dc=%d flag=%d\n", g_base_called, g_doit_called, g_doit_flag);
    return 0;
}