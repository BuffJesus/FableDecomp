#include <cstdio>

struct Member { int pad[16]; };
struct CEngineSubPrimitiveText {
    char pad[0x3c];
    Member m;
};

static int g_member_dtor = 0;
static int g_base_dtor = 0;
static int g_deleted = 0;

extern "C" void __fastcall Member_dtor(Member* self) { (void)self; g_member_dtor++; }
extern "C" void __fastcall Base_dtor(CEngineSubPrimitiveText* self) { (void)self; g_base_dtor++; }
extern "C" void __cdecl op_delete(void* p) { (void)p; g_deleted++; }

void* __fastcall CEngineSubPrimitiveText_scalar_deleting_destructor(CEngineSubPrimitiveText* self, int, unsigned int flags)
{
    Member_dtor(&self->m);
    Base_dtor(self);
    if (flags & 1)
        op_delete(self);
    return self;
}

int main()
{
    CEngineSubPrimitiveText obj;
    void* r = CEngineSubPrimitiveText_scalar_deleting_destructor(&obj, 0, 0);
    if (r != &obj || g_member_dtor != 1 || g_base_dtor != 1 || g_deleted != 0) {
        std::printf("FAIL no-delete path\n");
        return 1;
    }
    g_member_dtor = g_base_dtor = g_deleted = 0;
    r = CEngineSubPrimitiveText_scalar_deleting_destructor(&obj, 0, 1);
    if (r != &obj || g_member_dtor != 1 || g_base_dtor != 1 || g_deleted != 1) {
        std::printf("FAIL delete path\n");
        return 1;
    }
    std::printf("CEngineSubPrimitiveText_00456cff_TEST PASS\n");
    return 0;
}