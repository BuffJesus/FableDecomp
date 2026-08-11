#include <cstdio>

// Independent behaviour test: reproduce the derived-dtor sequence and observe
// vtable set, member destroyed, base destroyed, all in order.

static int g_order = 0;
static int g_vtbl_set = 0;
static int g_member_destroyed_order = 0;
static int g_base_destroyed_order = 0;

const int g_CInputProcessManager_vtbl = 0x129ab38;

struct MemberAt8 {
    void destroy() { g_member_destroyed_order = ++g_order; }
};

struct BaseInput {
    void baseDtor() { g_base_destroyed_order = ++g_order; }
};

struct CInputProcessManager {
    void* vtbl;
    int   pad;
    MemberAt8 member;
    void dtor_CInputProcessManager();
};

void CInputProcessManager::dtor_CInputProcessManager()
{
    this->vtbl = (void*)&g_CInputProcessManager_vtbl;
    g_vtbl_set = ++g_order;
    this->member.destroy();
    ((BaseInput*)this)->baseDtor();
}

int main()
{
    CInputProcessManager o;
    o.vtbl = 0;
    o.dtor_CInputProcessManager();

    bool ok = (o.vtbl == (void*)&g_CInputProcessManager_vtbl)
           && (g_vtbl_set == 1)
           && (g_member_destroyed_order == 2)
           && (g_base_destroyed_order == 3);

    if (ok) { printf("DTOR_SEQUENCE_OK\n"); return 0; }
    printf("DTOR_SEQUENCE_FAIL\n");
    return 1;
}