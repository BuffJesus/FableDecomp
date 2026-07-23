#include <cstdio>
struct CMember { };
struct CLandscapeBackgroundPatch {
    void* vtbl;
    int f4;
    void* f8;
    unsigned char pad[0x74 - 0xc];
    CMember member;
    void* CLandscapeBackgroundPatch_vector_deleting_destructor(unsigned char flags);
};

static int g_member_dtor = 0;
static int g_base_dtor = 0;
static int g_deleted = 0;
void* g_CLandscapeBackgroundPatch_vftable;

void __fastcall CMember_dtor(CMember* self) { (void)self; g_member_dtor++; }
void __fastcall CLandscapeBackgroundPatch_base_dtor(CLandscapeBackgroundPatch* self) { (void)self; g_base_dtor++; }
void __cdecl operator_delete(void* p) { (void)p; g_deleted++; }

void* CLandscapeBackgroundPatch::CLandscapeBackgroundPatch_vector_deleting_destructor(unsigned char flags)
{
    CMember_dtor(&this->member);
    this->vtbl = &g_CLandscapeBackgroundPatch_vftable;
    CLandscapeBackgroundPatch_base_dtor(this);
    if (flags & 1) {
        operator_delete(this);
    }
    return this;
}

int main() {
    CLandscapeBackgroundPatch obj;
    obj.vtbl = 0;
    void* r = obj.CLandscapeBackgroundPatch_vector_deleting_destructor(0);
    if (r != &obj || g_member_dtor != 1 || g_base_dtor != 1 || g_deleted != 0 || obj.vtbl != &g_CLandscapeBackgroundPatch_vftable) {
        std::printf("FAIL noflag\n"); return 1;
    }
    r = obj.CLandscapeBackgroundPatch_vector_deleting_destructor(1);
    if (r != &obj || g_member_dtor != 2 || g_base_dtor != 2 || g_deleted != 1) {
        std::printf("FAIL flag\n"); return 1;
    }
    std::printf("CLandscapeBackgroundPatch_0045470e_TEST PASS\n");
    return 0;
}