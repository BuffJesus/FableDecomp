#include <cstddef>
#include <cstdint>

class CCharString;

namespace NPlayerGui
{
class CDrawQuestInfo
{
public:
    long __thiscall AddCounter(
        const CCharString& labelText,
        long value,
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
    long __thiscall AddQuestInfoCounterList(
        const CCharString& labelText,
        long value,
        float fadeTime) const;
};

long __thiscall CGameScriptInterface::AddQuestInfoCounterList(
    const CCharString& labelText,
    long value,
    float fadeTime) const
{
    return DAT_013b8790->drawQuestInfo->AddCounter(labelText, value, fadeTime);
}