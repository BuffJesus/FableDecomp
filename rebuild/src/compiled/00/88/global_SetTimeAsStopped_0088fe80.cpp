#include <cstddef>
typedef unsigned char byte_t;

class CGameTimeManager
{
public:
    void SetTimeAsStopped(bool stopped, long& time);
};

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