#include <cstddef>
#include <cstdint>
#include <map>

namespace NUISystem
{
class CManager
{
public:
    enum EUsedKeys : long
    {
    };
};
}

class CGameScriptInterface
{
public:
    virtual long __thiscall RegisterTimer();
};

struct MapNodeOverlay
{
    std::byte pad_00[0x4];
    MapNodeOverlay* pParent; // 0x04
    MapNodeOverlay* pLeft;   // 0x08
    MapNodeOverlay* pRight;  // 0x0C
    long usedKey;            // 0x10
};

static_assert(offsetof(MapNodeOverlay, pParent) == 0x04);
static_assert(offsetof(MapNodeOverlay, pLeft) == 0x08);
static_assert(offsetof(MapNodeOverlay, pRight) == 0x0C);
static_assert(offsetof(MapNodeOverlay, usedKey) == 0x10);

struct TimerMapOverlay
{
    MapNodeOverlay* pHead;
};

static_assert(offsetof(TimerMapOverlay, pHead) == 0x00);

struct CGameScriptInterface_TimerMapOverlay
{
    std::byte pad_00[0x3C];
    TimerMapOverlay timerMapTree;
};

struct CGameScriptInterface_TimerStdMapOverlay
{
    std::byte pad_00[0x3C];
    std::map<NUISystem::CManager::EUsedKeys, long> timerMap;
};

static_assert(offsetof(CGameScriptInterface_TimerMapOverlay, timerMapTree) == 0x3C);
static_assert(offsetof(CGameScriptInterface_TimerStdMapOverlay, timerMap) == 0x3C);

long __thiscall CGameScriptInterface::RegisterTimer()
{
    auto* const pTreeOverlay =
        reinterpret_cast<CGameScriptInterface_TimerMapOverlay*>(this);
    auto* const pStdMapOverlay =
        reinterpret_cast<CGameScriptInterface_TimerStdMapOverlay*>(this);

    MapNodeOverlay* const pHead = pTreeOverlay->timerMapTree.pHead;
    long highestUsedKey = 0;
    MapNodeOverlay* pNode = pHead->pLeft;

    while (pNode != pHead)
    {
        if (highestUsedKey < pNode->usedKey)
        {
            highestUsedKey = pNode->usedKey;
        }

        MapNodeOverlay* pNext = pNode->pRight;
        if (pNext == nullptr)
        {
            pNext = pNode->pParent;
            if (pNode == pNext->pRight)
            {
                do
                {
                    pNode = pNext;
                    pNext = pNode->pParent;
                } while (pNode == pNext->pRight);
            }

            if (pNode->pRight != pNext)
            {
                pNode = pNext;
            }
        }
        else
        {
            MapNodeOverlay* pLeft = pNext->pLeft;
            pNode = pNext;
            while (pLeft != nullptr)
            {
                pNext = pLeft;
                pNode = pNext;
                pLeft = pNext->pLeft;
            }
        }
    }

    const NUISystem::CManager::EUsedKeys nextKey =
        static_cast<NUISystem::CManager::EUsedKeys>(highestUsedKey + 1);
    long* const pValue = &pStdMapOverlay->timerMap[nextKey];
    *pValue = 0;
    return highestUsedKey + 1;
}