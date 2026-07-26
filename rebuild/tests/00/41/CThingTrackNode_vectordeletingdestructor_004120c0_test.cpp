#include <cstdio>
#include <cstdlib>

struct CThingTrackNode {
    void* vptr;      // +0x0
    char pad4[0x0C]; // +0x4 .. +0x10
    void* member10;  // +0x10
    char pad14[0x0C];// +0x14 .. +0x20
    char member20;   // +0x20
};

static const void* CThingTrackNode_vtbl[4] = {0,0,0,0};
static int g_cleanup=0, g_subdtor=0, g_free=0, g_basedtor=0, g_opdelete=0;
static void* g_freed=0; static void* g_deleted=0;

void __fastcall Cleanup_5d72d0(void* self){ (void)self; g_cleanup++; }
void __fastcall SubDtor_589450(void* sub){ (void)sub; g_subdtor++; }
void Free_7ec954(void* p){ g_free++; g_freed=p; }
void __fastcall BaseDtor_588240(void* self){ (void)self; g_basedtor++; }
void OpDelete_7ec8fc(void* p){ g_opdelete++; g_deleted=p; }

void* __fastcall CThingTrackNode_vector_deleting_destructor(CThingTrackNode* self, void* /*edx*/, unsigned int flags)
{
    self->vptr = (void*)CThingTrackNode_vtbl;
    Cleanup_5d72d0(self);
    SubDtor_589450(&self->member20);
    if (self->member10)
        Free_7ec954(self->member10);
    BaseDtor_588240(self);
    if (flags & 1)
        OpDelete_7ec8fc(self);
    return self;
}

int main()
{
    // Case 1: member10 set, flags & 1 -> free + delete
    CThingTrackNode a;
    int dummy=0;
    a.vptr=(void*)0xDEAD;
    a.member10=&dummy;
    void* r = CThingTrackNode_vector_deleting_destructor(&a, 0, 1);
    if (r != &a) { std::printf("FAIL ret\n"); return 1; }
    if (a.vptr != (void*)CThingTrackNode_vtbl) { std::printf("FAIL vptr\n"); return 1; }
    if (g_cleanup!=1||g_subdtor!=1||g_basedtor!=1){ std::printf("FAIL calls\n"); return 1; }
    if (g_free!=1||g_freed!=&dummy){ std::printf("FAIL free\n"); return 1; }
    if (g_opdelete!=1||g_deleted!=&a){ std::printf("FAIL delete\n"); return 1; }

    // Case 2: member10 null, flags 0 -> no free, no delete
    CThingTrackNode b;
    b.member10=0;
    g_cleanup=g_subdtor=g_free=g_basedtor=g_opdelete=0;
    CThingTrackNode_vector_deleting_destructor(&b, 0, 0);
    if (g_free!=0){ std::printf("FAIL free2\n"); return 1; }
    if (g_opdelete!=0){ std::printf("FAIL delete2\n"); return 1; }
    if (g_cleanup!=1||g_subdtor!=1||g_basedtor!=1){ std::printf("FAIL calls2\n"); return 1; }

    std::printf("CThingTrackNode_004120c0_TEST PASS\n");
    return 0;
}