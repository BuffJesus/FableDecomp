#include <cstdio>

static int g_member_dtor = 0;
static int g_self_dtor = 0;
static int g_deleted = 0;
static char g_buf[0x80];

void __fastcall MemberAt3C_dtor(void* p) { g_member_dtor++; (void)p; }
void __fastcall Self_dtor(void* p) { g_self_dtor++; (void)p; }
void __cdecl operator_delete(void* p) { g_deleted++; (void)p; }

void* __fastcall CEngineSubPrimitiveText_scalar_deleting_destructor(char* self, int /*edx*/, unsigned int flags)
{
    MemberAt3C_dtor(self + 0x3c);
    Self_dtor(self);
    if (flags & 1)
        operator_delete(self);
    return self;
}

int main()
{
    // flags bit0 clear: no delete
    g_member_dtor = g_self_dtor = g_deleted = 0;
    void* r1 = CEngineSubPrimitiveText_scalar_deleting_destructor(g_buf, 0, 0);
    if (r1 != g_buf || g_member_dtor != 1 || g_self_dtor != 1 || g_deleted != 0) {
        std::printf("FAIL no-delete path\n");
        return 1;
    }
    // flags bit0 set: delete
    g_member_dtor = g_self_dtor = g_deleted = 0;
    void* r2 = CEngineSubPrimitiveText_scalar_deleting_destructor(g_buf, 0, 1);
    if (r2 != g_buf || g_member_dtor != 1 || g_self_dtor != 1 || g_deleted != 1) {
        std::printf("FAIL delete path\n");
        return 1;
    }
    std::printf("CEngineSubPrimitiveText_00456d43_TEST PASS\n");
    return 0;
}