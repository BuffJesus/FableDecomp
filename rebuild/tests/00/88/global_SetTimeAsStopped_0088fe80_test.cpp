#include <cstdio>
#include <cstddef>
typedef unsigned char byte_t;

static bool g_lastStopped = false;
static long g_lastTime = 0;
static void* g_lastThis = 0;

class CGameTimeManager
{
public:
    void SetTimeAsStopped(bool stopped, long& time);
};

void CGameTimeManager::SetTimeAsStopped(bool stopped, long& time)
{
    g_lastThis = this;
    g_lastStopped = stopped;
    g_lastTime = time;
}

class CGameScriptInterface
{
public:
    void SetTimeAsStopped(bool stopped, long& time) const;
};

namespace
{
struct CGameScriptInterface_SetTimeAsStopped_SelfOverlay
{
    byte_t m_Pad00[0x04];
    void* m_Field04; // 0x04
};

struct CGameScriptInterface_SetTimeAsStopped_Field04Overlay
{
    byte_t m_Pad00[0x1C];
    CGameTimeManager* m_Field1C; // 0x1C
};
}

void CGameScriptInterface::SetTimeAsStopped(
    bool stopped,
    long& time) const
{
    const CGameScriptInterface_SetTimeAsStopped_SelfOverlay* const self =
        reinterpret_cast<const CGameScriptInterface_SetTimeAsStopped_SelfOverlay*>(this);
    const CGameScriptInterface_SetTimeAsStopped_Field04Overlay* const field04 =
        reinterpret_cast<const CGameScriptInterface_SetTimeAsStopped_Field04Overlay*>(
            self->m_Field04);

    field04->m_Field1C->SetTimeAsStopped(stopped, time);
}

int main()
{
    CGameTimeManager timeMgr;

    CGameScriptInterface_SetTimeAsStopped_Field04Overlay field04Block;
    field04Block.m_Field1C = &timeMgr;

    CGameScriptInterface_SetTimeAsStopped_SelfOverlay selfBlock;
    selfBlock.m_Field04 = &field04Block;

    const CGameScriptInterface* const iface =
        reinterpret_cast<const CGameScriptInterface*>(&selfBlock);

    long timeVal = 12345;
    iface->SetTimeAsStopped(true, timeVal);


    bool ok = (g_lastThis == static_cast<void*>(&timeMgr)) &&
              (g_lastStopped == true) &&
              (g_lastTime == 12345);

    if (ok)
    {
        std::printf("SET_TIME_AS_STOPPED_OK\n");
    }
    else
    {
        std::printf("SET_TIME_AS_STOPPED_FAIL\n");
    }

    return 0;
}