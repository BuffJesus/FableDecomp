#include <stdio.h>

struct CBackgroundTesselator
{
    int calls;
    void Update();
};

struct CEngineLandscapeMap
{
    int m_pad0;                    // +0x00
    CBackgroundTesselator* m_tess; // +0x04
    char m_fill[0x54];             // +0x08 .. +0x5b
    char m_updating;               // +0x5c
    void UpdateBackgroundTesselation();
};

static CEngineLandscapeMap* g_seen = 0;

void CBackgroundTesselator::Update()
{
    this->calls++;
    // during the call the guard flag must be set
    if (g_seen && g_seen->m_updating == 1)
        this->calls += 100;
}

void CEngineLandscapeMap::UpdateBackgroundTesselation()
{
    CBackgroundTesselator* t = this->m_tess;
    this->m_updating = 1;
    if (t)
        t->Update();
    this->m_updating = 0;
}

int main()
{
    // case 1: null tesselator -> no call, flag ends 0
    CEngineLandscapeMap m1;
    m1.m_tess = 0;
    m1.m_updating = 9;
    m1.UpdateBackgroundTesselation();
    bool ok1 = (m1.m_updating == 0);

    // case 2: real tesselator -> Update called once, flag was 1 during, 0 after
    CBackgroundTesselator t;
    t.calls = 0;
    CEngineLandscapeMap m2;
    m2.m_tess = &t;
    m2.m_updating = 9;
    g_seen = &m2;
    m2.UpdateBackgroundTesselation();
    bool ok2 = (t.calls == 101) && (m2.m_updating == 0);

    if (ok1 && ok2)
        printf("TESSELATION_GUARD_OK\n");
    else
        printf("FAIL ok1=%d ok2=%d calls=%d flag=%d\n", ok1, ok2, t.calls, m2.m_updating);
    return (ok1 && ok2) ? 0 : 1;
}