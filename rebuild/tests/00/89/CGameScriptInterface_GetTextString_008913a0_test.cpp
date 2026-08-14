#include <cstdio>

struct CCharString { const char* p; };
struct CWideString { const wchar_t* p; };
struct CGameScriptInterface { char pad[4]; };

struct Sub { CWideString stored; };
static Sub g_sub;
struct Obj { char pad[0x14]; Sub* sub; };
static Obj g_obj_inst;
static Obj* g_obj_ptr = &g_obj_inst;

static CWideString* HelperImpl(Sub* self, CWideString* ret, CCharString const& str)
{
    // model: copy something into ret from the sub, keyed off str
    ret->p = self->stored.p;
    return ret;
}

static CWideString* Model(CGameScriptInterface* self, CWideString* ret, CCharString const& str)
{
    (void)self;
    Sub* sub = g_obj_ptr->sub; // +0x14
    HelperImpl(sub, ret, str);
    return ret;
}

int main()
{
    g_sub.stored.p = L"hello";
    g_obj_inst.sub = &g_sub;

    CGameScriptInterface gsi;
    CCharString key; key.p = "k";
    CWideString out; out.p = 0;
    CWideString* r = Model(&gsi, &out, key);

    bool ok = (r == &out) && (out.p != 0) && (out.p[0] == L'h');
    if (ok) printf("PARITY_OK\n");
    else printf("FAIL\n");
    return 0;
}