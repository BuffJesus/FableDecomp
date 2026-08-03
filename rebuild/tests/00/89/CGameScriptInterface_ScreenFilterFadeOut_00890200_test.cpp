#include <cstdio>

struct Field18Overlay;

static int g_calls = 0;
static void* g_seenThis = 0;
static float g_seenTime = 0.0f;
static void* g_seenCtx = 0;

class CXMVPlaybackInfo
{
public:
    void FadeOut(float fadeOutTime, void* pFadeOutContext)
    {
        ++g_calls;
        g_seenThis = (void*)this;
        g_seenTime = fadeOutTime;
        g_seenCtx = pFadeOutContext;
    }
};

struct Field18Overlay
{
    char m_Pad00[0x1964];
    CXMVPlaybackInfo* m_pPlaybackInfo;
};

struct Field04Overlay
{
    char m_Pad00[0x18];
    Field18Overlay* m_Field18;
};

struct SelfOverlay
{
    char m_Pad00[0x04];
    Field04Overlay* m_Field04;
};

class CGameScriptInterface
{
public:
    virtual void ScreenFilterFadeOut(float fadeOutTime, void* pFadeOutContext) const;
};

void CGameScriptInterface::ScreenFilterFadeOut(float fadeOutTime, void* pFadeOutContext) const
{
    const SelfOverlay* self = (const SelfOverlay*)this;
    Field04Overlay* field04 = self->m_Field04;
    Field18Overlay* field18 = field04->m_Field18;
    field18->m_pPlaybackInfo->FadeOut(fadeOutTime, pFadeOutContext);
}

int main()
{
    CXMVPlaybackInfo playback;

    Field18Overlay f18;
    f18.m_pPlaybackInfo = &playback;

    Field04Overlay f04;
    f04.m_Field18 = &f18;

    void* rawSelf[2];
    rawSelf[0] = 0;
    rawSelf[1] = &f04;

    CGameScriptInterface* pIface = (CGameScriptInterface*)&rawSelf[0];

    void* ctx = (void*)0x0BADF00D;
    float t = 2.5f;
    pIface->CGameScriptInterface::ScreenFilterFadeOut(t, ctx);

    if (g_calls == 1 && g_seenThis == (void*)&playback && g_seenTime == 2.5f && g_seenCtx == ctx)
        printf("SCREENFADE_OK\n");
    else
        printf("SCREENFADE_FAIL calls=%d this=%p time=%f\n", g_calls, g_seenThis, g_seenTime);
    return 0;
}