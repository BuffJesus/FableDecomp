#include <cstdio>

struct VolObj;
typedef void (__stdcall *VolFn)(VolObj*);

struct VolVtbl
{
    VolFn f0;
    VolFn f1;
    VolFn release;
};

struct VolObj
{
    VolVtbl* vtbl;
    int released;
};

struct CVolume
{
    int pad0;
    VolObj* m_vol;
    void ReleaseVolume();
};

void CVolume::ReleaseVolume()
{
    VolObj* v = m_vol;
    if (v)
    {
        v->vtbl->release(v);
        m_vol = 0;
    }
}

static int g_calls = 0;
static VolObj* g_lastArg = 0;

static void __stdcall do_release(VolObj* p)
{
    g_calls++;
    g_lastArg = p;
    p->released = 1;
}

int main()
{
    VolVtbl vt;
    vt.f0 = 0;
    vt.f1 = 0;
    vt.release = do_release;

    VolObj obj;
    obj.vtbl = &vt;
    obj.released = 0;

    CVolume vol;
    vol.pad0 = 0x12345678;
    vol.m_vol = &obj;

    vol.ReleaseVolume();

    bool ok = true;
    if (g_calls != 1) ok = false;
    if (g_lastArg != &obj) ok = false;
    if (obj.released != 1) ok = false;
    if (vol.m_vol != 0) ok = false;

    // null case should be a no-op
    CVolume vol2;
    vol2.pad0 = 0;
    vol2.m_vol = 0;
    vol2.ReleaseVolume();
    if (g_calls != 1) ok = false;
    if (vol2.m_vol != 0) ok = false;

    if (ok) printf("VOLRELEASE_OK\n");
    else printf("VOLRELEASE_FAIL calls=%d\n", g_calls);
    return ok ? 0 : 1;
}