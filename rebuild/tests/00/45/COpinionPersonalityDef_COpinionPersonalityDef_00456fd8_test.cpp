#include <cstdio>

static int g_freed = 0;
static int g_member = 0;
static int g_base = 0;
static int g_vtbl_val = 0x1233f8c;
const int g_vtbl = 0;

struct COpinionPersonalityDef {
    void* vtbl;
    char pad[0x3c-4];
    void* ptr;
    char member[8];
};

void __cdecl free_field(void* p) { if (p) g_freed = 1; }
void __fastcall member_dtor(void* self) { g_member = 1; }
void __fastcall base_dtor(void* self) { g_base = 1; }

void __fastcall COpinionPersonalityDef_dtor(COpinionPersonalityDef* self)
{
    self->vtbl = (void*)&g_vtbl;
    free_field(self->ptr);
    self->ptr = 0;
    member_dtor((char*)self + 0x44);
    base_dtor(self);
}

int main()
{
    COpinionPersonalityDef obj;
    obj.vtbl = 0;
    obj.ptr = (void*)0x1234;
    COpinionPersonalityDef_dtor(&obj);
    if (g_freed == 1 && obj.ptr == 0 && g_member == 1 && g_base == 1 &&
        obj.vtbl == (void*)&g_vtbl) {
        std::printf("COpinionPersonalityDef_00456fd8_TEST PASS\n");
        return 0;
    }
    std::printf("FAIL freed=%d member=%d base=%d ptr=%p\n", g_freed, g_member, g_base, obj.ptr);
    return 1;
}