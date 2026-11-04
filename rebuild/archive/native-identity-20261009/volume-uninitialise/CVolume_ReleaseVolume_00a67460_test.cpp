#include <stdio.h>

struct IRefObject;

struct IVTable
{
    void (__stdcall *slot0)(IRefObject* self);
    void (__stdcall *slot1)(IRefObject* self);
    void (__stdcall *slot2)(IRefObject* self);
};

struct IRefObject
{
    IVTable* vptr;
};

struct CVolume
{
    void*       m_pad0;
    IRefObject* m_pObject;
    void ReleaseVolume(int unused);
};

void CVolume::ReleaseVolume(int)
{
    IRefObject* p = m_pObject;
    if (p != 0)
    {
        p->vptr->slot2(p);
        m_pObject = 0;
    }
}

static int g_released = 0;
static IRefObject* g_lastArg = 0;

static void __stdcall slot0(IRefObject*) {}
static void __stdcall slot1(IRefObject*) {}
static void __stdcall slot2(IRefObject* p) { g_released++; g_lastArg = p; }

int main()
{
    IVTable vt;
    vt.slot0 = slot0;
    vt.slot1 = slot1;
    vt.slot2 = slot2;

    IRefObject obj;
    obj.vptr = &vt;

    CVolume v;
    v.m_pad0 = (void*)0xDEAD;
    v.m_pObject = &obj;

    v.ReleaseVolume(0);
    if (g_released != 1) { printf("FAIL released\n"); return 1; }
    if (g_lastArg != &obj) { printf("FAIL arg\n"); return 1; }
    if (v.m_pObject != 0) { printf("FAIL notnull\n"); return 1; }

    v.m_pObject = 0;
    v.ReleaseVolume(0);
    if (g_released != 1) { printf("FAIL nullcall\n"); return 1; }
    if (v.m_pObject != 0) { printf("FAIL still\n"); return 1; }

    printf("RELEASEVOLUME_OK\n");
    return 0;
}