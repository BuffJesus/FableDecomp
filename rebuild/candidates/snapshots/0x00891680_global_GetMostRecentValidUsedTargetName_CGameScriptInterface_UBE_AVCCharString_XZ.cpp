#include <cstddef>

class CCharString;
class CGameScriptInterface;

class CCharString
{
public:
    CCharString(const CCharString& other);
};

namespace
{
struct MostRecentValidUsedTargetNameGlobalsOverlay
{
    std::byte m_Pad00[0xB0];
    CCharString m_MostRecentValidUsedTargetName; // 0xB0
};

static_assert(
    offsetof(MostRecentValidUsedTargetNameGlobalsOverlay, m_MostRecentValidUsedTargetName) ==
    0xB0);

extern MostRecentValidUsedTargetNameGlobalsOverlay* DAT_013b89fc;
}

class CGameScriptInterface
{
public:
    CCharString GetMostRecentValidUsedTargetName() const;
};

CCharString CGameScriptInterface::GetMostRecentValidUsedTargetName() const
{
    return CCharString(DAT_013b89fc->m_MostRecentValidUsedTargetName);
}