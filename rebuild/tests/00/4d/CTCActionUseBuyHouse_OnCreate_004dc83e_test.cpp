#include <cstdio>

struct CMemberArg;

struct Registrar
{
    void Register(CMemberArg* p);
};

struct Holder
{
    char        pad[0x70];
    Registrar*  slot70;
};

struct CTCActionUseBuyHouse
{
    void*   v0;
    Holder* m4;
    char    pad8[4];
    char    member0c;

    void OnCreate();
    void BaseOnCreate();
};

// ---- observable side effects ----
static void*  g_registeredArg = 0;
static void*  g_registrarThis = 0;
static void*  g_baseThis      = 0;

void Registrar::Register(CMemberArg* p)
{
    g_registrarThis = this;
    g_registeredArg = (void*)p;
}

void CTCActionUseBuyHouse::BaseOnCreate()
{
    g_baseThis = this;
}

int main()
{
    Registrar theRegistrar[1];
    Registrar* pRegistrar = theRegistrar;
    Holder    h;
    h.slot70 = pRegistrar;

    CTCActionUseBuyHouse obj;
    obj.v0 = 0;
    obj.m4 = &h;

    obj.OnCreate();

    bool ok = true;
    // Register called with this == this->m4->slot70
    if (g_registrarThis != (void*)pRegistrar) ok = false;
    // arg == &obj.member0c
    if (g_registeredArg != (void*)&obj.member0c) ok = false;
    // base tail call ran with this == &obj
    if (g_baseThis != (void*)&obj) ok = false;

    printf(ok ? "ONCREATE_OK\n" : "ONCREATE_FAIL\n");
    return ok ? 0 : 1;
}