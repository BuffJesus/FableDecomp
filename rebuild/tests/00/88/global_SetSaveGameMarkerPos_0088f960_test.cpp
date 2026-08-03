struct C3DVector
{
    float x;
    float y;
    float z;
};

static C3DVector g_lastPos;
static void* g_lastWorld;
static int g_callCount;

class CWorld
{
public:
    void SetSaveGameMarkerPos(const C3DVector* pos)
    {
        g_lastWorld = this;
        g_lastPos = *pos;
        g_callCount++;
    }
};

class CGameScriptInterface
{
public:
    void SetSaveGameMarkerPos(const C3DVector& pos);
    char m_Pad[0x4];
    CWorld* m_World;
};

void CGameScriptInterface::SetSaveGameMarkerPos(const C3DVector& pos)
{
    m_World->SetSaveGameMarkerPos(&pos);
}

#include <cstdio>

int main()
{
    CWorld world;
    CGameScriptInterface gsi;
    gsi.m_World = &world;

    C3DVector pos;
    pos.x = 1.0f;
    pos.y = 2.0f;
    pos.z = 3.0f;

    gsi.SetSaveGameMarkerPos(pos);

    if (g_callCount == 1 && g_lastWorld == (void*)&world &&
        g_lastPos.x == 1.0f && g_lastPos.y == 2.0f && g_lastPos.z == 3.0f)
    {
        printf("TEST_PASS_MARKER_OK\n");
    }
    else
    {
        printf("TEST_FAIL\n");
    }
    return 0;
}