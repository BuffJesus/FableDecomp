#include <cstdio>

struct Member { void* vp; };
struct Base { void* vp; };
struct CFileDataInputStream {
    void* vptr;
    Member member;
};

static int g_member_dtor_calls = 0;
static int g_base_dtor_calls = 0;
static void* g_vptr_at_member_dtor = 0;
static void* g_vptr_at_base_dtor = 0;
static Member* g_member_arg = 0;
static Base* g_base_arg = 0;

#define VT_DERIVED ((void*)0x0129a6f8)
#define VT_BASE    ((void*)0x0129a69c)

// stand-ins recording observable behaviour
void __fastcall Member_dtor(Member* self) { g_member_dtor_calls++; g_member_arg = self; }
void __fastcall Base_dtor(Base* self) { g_base_dtor_calls++; g_base_arg = self; }

// mirror of the reconstructed body, with instrumentation of vptr at each call
void __fastcall dtor(CFileDataInputStream* self)
{
    self->vptr = VT_DERIVED;
    g_vptr_at_member_dtor = self->vptr;
    Member_dtor(&self->member);
    self->vptr = VT_BASE;
    g_vptr_at_base_dtor = self->vptr;
    Base_dtor((Base*)self);
}

int main()
{
    CFileDataInputStream obj;
    obj.vptr = 0;
    obj.member.vp = 0;
    dtor(&obj);

    bool ok = true;
    ok = ok && (g_member_dtor_calls == 1);
    ok = ok && (g_base_dtor_calls == 1);
    ok = ok && (g_vptr_at_member_dtor == VT_DERIVED);
    ok = ok && (g_vptr_at_base_dtor == VT_BASE);
    // member dtor receives &obj.member = self+4
    ok = ok && (g_member_arg == (Member*)((char*)&obj + 4));
    // base dtor receives self (offset 0)
    ok = ok && (g_base_arg == (Base*)&obj);
    // final vptr state is base vtable
    ok = ok && (obj.vptr == VT_BASE);

    if (ok) printf("DTOR_PARITY_OK\n");
    else printf("DTOR_PARITY_FAIL m=%d b=%d\n", g_member_dtor_calls, g_base_dtor_calls);
    return 0;
}