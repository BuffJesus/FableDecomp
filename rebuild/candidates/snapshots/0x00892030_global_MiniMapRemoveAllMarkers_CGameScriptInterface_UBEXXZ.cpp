#include <cstdint>
#include <utility>

class CVertexBufferWin32
{
public:
    void* __thiscall DoSizeof();
};

class CParticleMorphs
{
public:
    class CEntry;
};

enum EHeroMorphType : std::int32_t;

namespace std
{
template <class Allocator, class ValueType, class ReferenceType>
void __thiscall _Cons_val(
    Allocator* pAllocator,
    ReferenceType value);
}

class CGameScriptInterface
{
public:
    void __thiscall MiniMapRemoveAllMarkers() const;
};

namespace
{
using PairType = std::pair<EHeroMorphType, CParticleMorphs::CEntry>;
using AllocatorType = std::allocator<PairType>;

inline CVertexBufferWin32*& DAT_013B8790 =
    *reinterpret_cast<CVertexBufferWin32**>(0x013B8790);

inline constexpr auto pDoSizeof =
    reinterpret_cast<void* (CVertexBufferWin32::*)()>(&CVertexBufferWin32::DoSizeof);

inline constexpr auto p_Cons_val =
    &std::_Cons_val<AllocatorType, PairType, const PairType&>;
}

__declspec(naked) void __thiscall CGameScriptInterface::MiniMapRemoveAllMarkers() const
{
    __asm
    {
        mov ecx, dword ptr [DAT_013B8790]
        call pDoSizeof
        mov ecx, eax
        jmp p_Cons_val
    }
}