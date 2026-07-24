#include <cstddef>
#include <cstdint>
#include <cstdlib>

class CRegion
{
public:
    void __thiscall SetMiniMapRegionExitTextOffsetX(void** pFoundNode, const long* pTimerId);
};

extern "C" void* __stdcall CMemoryAllocatorVariableSize__Erase(
    void* pNode,
    void* pField04,
    void* pField08,
    void* pField0C);

class CGameScriptInterface
{
public:
    void __thiscall DeregisterTimer(long timerId);
};

namespace
{
struct CGameScriptInterface_DeregisterTimer_Overlay
{
    std::byte m_Pad00[0x3C];
    void* m_Field3C;        // 0x3C
    std::int32_t m_Field40; // 0x40
};

static_assert(offsetof(CGameScriptInterface_DeregisterTimer_Overlay, m_Field3C) == 0x3C);
static_assert(offsetof(CGameScriptInterface_DeregisterTimer_Overlay, m_Field40) == 0x40);
}

void __thiscall CGameScriptInterface::DeregisterTimer(long timerId)
{
    auto* const self = reinterpret_cast<CGameScriptInterface_DeregisterTimer_Overlay*>(this);

    void* foundNode = this;
    reinterpret_cast<CRegion*>(reinterpret_cast<std::byte*>(this) + 0x3C)
        ->SetMiniMapRegionExitTextOffsetX(&foundNode, &timerId);

    if (foundNode != self->m_Field3C)
    {
        auto* const field3CBytes = reinterpret_cast<std::byte*>(self->m_Field3C);
        void* const pMemory = CMemoryAllocatorVariableSize__Erase(
            foundNode,
            field3CBytes + 0x04,
            field3CBytes + 0x08,
            field3CBytes + 0x0C);

        if (pMemory != nullptr)
        {
            std::free(pMemory);
        }

        self->m_Field40 = self->m_Field40 + -1;
    }
}