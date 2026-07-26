#include <cstdio>

struct CMember { };
static int g_vtbl = 0;
static int member_calls = 0;
static int base_calls = 0;
static int delete_calls = 0;

void __fastcall CMember_dtor(CMember* self) { (void)self; member_calls++; }
void __fastcall Base_dtor(void* self) { (void)self; base_calls++; }
void __cdecl op_delete(void* p) { (void)p; delete_calls++; }

struct CLandscapeBackgroundPatch {
    int f0;
    int f4;
    void* f8;
    char pad[0x28 - 0xc];
    CMember m28;
};

void* __fastcall CLandscapeBackgroundPatch_vector_deleting_destructor(CLandscapeBackgroundPatch* self, int edx, unsigned char flags)
{
    CMember_dtor(&self->m28);
    *(const int**)self = &g_vtbl;
    Base_dtor(self);
    if (flags & 1)
        op_delete(self);
    return self;
}

int main()
{
    CLandscapeBackgroundPatch obj;
    obj.f0 = 123;

    // flags bit0 clear: no delete
    member_calls = base_calls = delete_calls = 0;
    void* r = CLandscapeBackgroundPatch_vector_deleting_destructor(&obj, 0, 0);
    if (r != &obj) { std::printf("FAIL ret\n"); return 1; }
    if (member_calls != 1) { std::printf("FAIL member\n"); return 1; }
    if (base_calls != 1) { std::printf("FAIL base\n"); return 1; }
    if (delete_calls != 0) { std::printf("FAIL delete-noflag\n"); return 1; }
    if (*(int*)&obj != (int)(long)&g_vtbl) { std::printf("FAIL vtbl\n"); return 1; }

    // flags bit0 set: delete
    member_calls = base_calls = delete_calls = 0;
    r = CLandscapeBackgroundPatch_vector_deleting_destructor(&obj, 0, 1);
    if (delete_calls != 1) { std::printf("FAIL delete-flag\n"); return 1; }

    std::printf("CLandscapeBackgroundPatch_00454647_TEST PASS\n");
    return 0;
}