#include <cstdio>
#include <cstring>

typedef signed int   fable_i32;

struct CDisplayRenderTarget;
struct CDisplaySubsystem;
struct CDisplayOwnerControl;

typedef void (__fastcall *RtSlot)(CDisplayRenderTarget* self);
struct CDisplayRenderTarget {
    RtSlot* vtbl;   // +0x0
};

struct CDisplaySubsystem {
    int dummy;
};

struct CDisplayOwnerControl {
    fable_i32 owners;                              // +0x0
    void (__fastcall *destroy)(void* object);      // +0x4
    void* object;                                  // +0x8
};

struct CDisplayEngine {
    void*                 vptr;                     // +0x00
    char                  pad_04[0x2C - 0x04];
    CDisplayRenderTarget* m_renderTarget;           // +0x2C
    char                  pad_30[0xE0 - 0x30];
    void*                 m_ownerPtr;               // +0xE0
    CDisplayOwnerControl* m_ownerRef;               // +0xE4
};

static int   g_order;
static int   g_shutdownSeq, g_flushSeq, g_destroySeq, g_freeSeq;
static void* g_shutdownSelf;
static CDisplaySubsystem* g_flushSelf;
static void* g_destroyThis;
static void* g_freed;
static int   g_getSub;
static CDisplayRenderTarget* g_getSubArg;
static CDisplaySubsystem g_subInstance;

static void __fastcall RenderTarget_Shutdown(CDisplayRenderTarget* self){
    g_shutdownSelf = (void*)self; g_shutdownSeq = ++g_order;
}
static void __fastcall Subsystem_Flush(CDisplaySubsystem* self){
    g_flushSelf = self; g_flushSeq = ++g_order;
}
CDisplaySubsystem* __stdcall FableDisplayGetSubsystem_004354c0(CDisplayRenderTarget* rt){
    ++g_getSub; g_getSubArg = rt; return (&g_subInstance);
}
extern "C" void __cdecl FableDisplayFree_004354c0(void* block){
    g_freed = block; g_freeSeq = ++g_order;
}
static void __fastcall OwnerDestroy(void* object){
    g_destroyThis = object; g_destroySeq = ++g_order;
}

void __fastcall CDisplayEngine_dtor(CDisplayEngine* thisptr){
    thisptr->m_renderTarget->vtbl[3](thisptr->m_renderTarget);
    Subsystem_Flush(FableDisplayGetSubsystem_004354c0(thisptr->m_renderTarget));
    if (thisptr->m_ownerRef) {
        if (--thisptr->m_ownerRef->owners == 0) {
            thisptr->m_ownerRef->destroy(thisptr->m_ownerRef->object);
            FableDisplayFree_004354c0(thisptr->m_ownerRef);
        }
    }
    thisptr->m_ownerPtr = 0;
    thisptr->m_ownerRef = 0;
}

static RtSlot g_rtVtbl[4] = { 0, 0, 0, &RenderTarget_Shutdown };

static bool run(int refs, bool present){
    if (((char*)&((CDisplayEngine*)0)->m_renderTarget - (char*)0) != 0x2C) return false;
    if (((char*)&((CDisplayEngine*)0)->m_ownerPtr    - (char*)0) != 0xE0) return false;
    if (((char*)&((CDisplayEngine*)0)->m_ownerRef    - (char*)0) != 0xE4) return false;

    CDisplayEngine e;
    std::memset(&e, 0, sizeof(e));

    CDisplayRenderTarget rt;
    CDisplayRenderTarget* rtp = (&rt);
    rt.vtbl = g_rtVtbl;
    e.m_renderTarget = rtp;
    e.m_ownerPtr = (void*)0x99887766;

    CDisplayOwnerControl ctrl;
    CDisplayOwnerControl* ctrlp = (&ctrl);
    ctrl.owners  = refs;
    ctrl.destroy = OwnerDestroy;
    ctrl.object  = (void*)0x11223344;
    e.m_ownerRef = present ? ctrlp : 0;

    g_order = 0;
    g_shutdownSeq = g_flushSeq = g_destroySeq = g_freeSeq = 0;
    g_shutdownSelf = 0; g_flushSelf = 0; g_destroyThis = 0; g_freed = 0;
    g_getSub = 0; g_getSubArg = 0;

    CDisplayEngine_dtor(&e);

    if (g_shutdownSeq != 1 || g_shutdownSelf != (void*)rtp) return false;
    if (g_getSub != 1 || g_getSubArg != rtp) return false;
    if (g_flushSeq != 2 || g_flushSelf != (&g_subInstance)) return false;
    if (e.m_ownerPtr != 0 || e.m_ownerRef != 0) return false;

    if (!present)
        return g_destroySeq == 0 && g_freeSeq == 0;
    if (refs == 2)
        return ctrl.owners == 1 && g_destroySeq == 0 && g_freeSeq == 0;
    return ctrl.owners == 0
        && g_destroySeq == 3 && g_destroyThis == (void*)0x11223344
        && g_freeSeq == 4 && g_freed == (void*)ctrlp;
}

int main(){
    bool a = run(0, false), b = run(2, true), c = run(1, true);
    if (!a || !b || !c){
        std::printf("FAITHFUL_004354c0 FAIL %d%d%d\n", a, b, c);
        return 1;
    }
    std::puts("FAITHFUL_004354c0 PASS");
    return 0;
}