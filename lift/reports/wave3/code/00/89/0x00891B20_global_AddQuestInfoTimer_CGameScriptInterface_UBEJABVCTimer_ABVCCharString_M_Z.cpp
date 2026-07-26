#include <cstddef>
#include <cstdint>

class CTimer;
class CCharString;

namespace NPlayerGui
{
class CDrawQuestInfo
{
public:
    long __thiscall AddCounter(
        const CTimer& timer,
        const CCharString& labelText,
        float fadeTime);
};
}

struct DAT_013b8790_Overlay
{
    std::byte pad_0000[0x1C8];
    NPlayerGui::CDrawQuestInfo* drawQuestInfo;
};

static_assert(offsetof(DAT_013b8790_Overlay, drawQuestInfo) == 0x1C8);

extern DAT_013b8790_Overlay* DAT_013b8790;

class CGameScriptInterface
{
public:
    long __thiscall AddQuestInfoTimer(
        const CTimer& timer,
        const CCharString& labelText,
        float fadeTime) const;
};

long __thiscall CGameScriptInterface::AddQuestInfoTimer(
    const CTimer& timer,
    const CCharString& labelText,
    float fadeTime) const
{
    return DAT_013b8790->drawQuestInfo->AddCounter(timer, labelText, fadeTime);
}