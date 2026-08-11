#include <cstdio>

// Self-contained behaviour model mirroring the destructor chain order.
static int g_seq;
static int g_member_destroyed;
static int g_base_destroyed;
static void* g_vtbl_written;

struct MemberObj {
    void* p;
    MemberObj(){ p = 0; }
    ~MemberObj(){ g_member_destroyed = ++g_seq; }
};

struct BaseProc {
    void* vtbl;
    char pad[0xa0];
    BaseProc(){ vtbl = 0; }
    virtual ~BaseProc(){ g_base_destroyed = ++g_seq; }
};

struct CInputProcessManager : public BaseProc {
    MemberObj member;
    CInputProcessManager(){}
    virtual ~CInputProcessManager(){
        g_vtbl_written = (void*)0x124dddc;   // conceptual own vtable write
        // member destroyed next (implicit), base last (implicit)
    }
};

int main(){
    {
        CInputProcessManager m;
    }
    // Expected order: derived body (vtbl write) -> member dtor -> base dtor
    bool ok = (g_vtbl_written == (void*)0x124dddc)
           && (g_member_destroyed == 1)   // member first after body
           && (g_base_destroyed == 2);    // base last
    if(ok) printf("DTOR_CHAIN_OK\n");
    else   printf("DTOR_CHAIN_FAIL m=%d b=%d\n", g_member_destroyed, g_base_destroyed);
    return ok?0:1;
}