#include <cstdio>

struct CThingTrackNode {
    void* vptr;
    char pad[0x18 - 4];
    int member18;
};

static const void* CThingTrackNode_vftable = (const void*)0x122d06c;
static int g_599270 = 0;
static int g_m18 = 0;
static int g_base = 0;
static int g_freed = 0;

void __fastcall Member599270(void* self) { (void)self; g_599270++; }
void __fastcall Member18Dtor(void* p) { (void)p; g_m18++; }
void __fastcall BaseDtor(void* self) { (void)self; g_base++; }
void __cdecl EngineFree(void* p) { (void)p; g_freed++; }

void* __fastcall CThingTrackNode_vector_deleting_destructor(CThingTrackNode* self, int edx, unsigned int flags)
{
    (void)edx;
    self->vptr = (void*)CThingTrackNode_vftable;
    Member599270(self);
    Member18Dtor((char*)self + 0x18);
    BaseDtor(self);
    if (flags & 1) {
        EngineFree(self);
    }
    return self;
}

int main() {
    CThingTrackNode a;
    a.vptr = 0;
    void* r = CThingTrackNode_vector_deleting_destructor(&a, 0, 0);
    if (r != &a || a.vptr != (void*)CThingTrackNode_vftable) { std::printf("FAIL retval/vptr\n"); return 1; }
    if (g_599270 != 1 || g_m18 != 1 || g_base != 1 || g_freed != 0) { std::printf("FAIL nofree calls\n"); return 1; }

    CThingTrackNode b;
    b.vptr = 0;
    CThingTrackNode_vector_deleting_destructor(&b, 0, 1);
    if (g_freed != 1) { std::printf("FAIL free-flag\n"); return 1; }

    std::printf("CThingTrackNode_004016b0_TEST PASS\n");
    return 0;
}