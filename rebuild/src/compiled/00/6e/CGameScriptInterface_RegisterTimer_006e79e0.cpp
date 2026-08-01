#pragma auto_inline(off)
#include <cstddef>
#include <map>

namespace NUISystem
{
class CManager
{
public:
    enum EUsedKeys
    {
        EUsedKeys_dummy = 0
    };
};
}

class CGameScriptInterface
{
public:
    virtual long __fastcall RegisterTimer();
};

struct MapNodeOverlay
{
    char pad_00[0x4];
    MapNodeOverlay* pParent; // 0x04
    MapNodeOverlay* pLeft;   // 0x08
    MapNodeOverlay* pRight;  // 0x0C
    long usedKey;            // 0x10
};

struct TimerMapOverlay
{
    MapNodeOverlay* pHead;
};

struct CGameScriptInterface_TimerMapOverlay
{
    char pad_00[0x3C];
    TimerMapOverlay timerMapTree;
};

struct CGameScriptInterface_TimerStdMapOverlay
{
    char pad_00[0x3C];
    std::map<NUISystem::CManager::EUsedKeys, long> timerMap;
};

long __fastcall CGameScriptInterface::RegisterTimer()
{
    typedef std::map<NUISystem::CManager::EUsedKeys, long> Map;
    Map* const pMap =
        reinterpret_cast<Map*>(reinterpret_cast<char*>(this) + 0x3C);

    MapNodeOverlay* const pHead =
        reinterpret_cast<TimerMapOverlay*>(pMap)->pHead;
    long highestUsedKey = 0;
    MapNodeOverlay* pNode = pHead->pLeft;

    while (pNode != pHead)
    {
        if (pNode->usedKey > highestUsedKey)
        {
            highestUsedKey = pNode->usedKey;
        }

        MapNodeOverlay* pScratch;
        if (pNode->pRight != 0)
        {
            pNode = pNode->pRight;
            while (pNode->pLeft != 0)
            {
                pNode = pNode->pLeft;
            }
        }
        else
        {
            pScratch = pNode->pParent;
            if (pNode == pScratch->pRight)
            {
                do
                {
                    pNode = pScratch;
                    pScratch = pNode->pParent;
                } while (pNode == pScratch->pRight);
            }

            if (pNode->pRight != pScratch)
            {
                pNode = pScratch;
            }
        }
    }

    const long nextKey = highestUsedKey + 1;
    (*pMap)[static_cast<NUISystem::CManager::EUsedKeys>(nextKey)] = 0;
    return nextKey;
}