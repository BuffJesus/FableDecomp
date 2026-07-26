#include <cstddef>
#include <cstdint>
#include <windows.h>

class CInputManagerDX;
class CGameEventPackage;
class CMouseDX;

extern "C" CMouseDX* __thiscall NHeroInformationScreens_Mouse_Constructor(
    CMouseDX* self,
    CInputManagerDX& inputManager,
    bool exclusive);

extern "C" void __thiscall _SetExclusiveMode_CMouseDX__UAE_N_N_Z(
    CMouseDX* self,
    bool exclusive);

extern void* PTR__vector_deleting_destructor__0129ea14;

class CGameEventPackage
{
public:
    static void Clear(CGameEventPackage* package);
};

class CMouseDX
{
public:
    CMouseDX(CInputManagerDX& inputManager, bool exclusive);
};

namespace
{
struct CMouseDX_Overlay
{
    void* vftable;
    std::byte pad_0004[0x3408 - 0x0004];
    std::int32_t field_3408;
    std::byte pad_340C[0x341C - 0x340C];
    std::int32_t field_341C;
    CInputManagerDX* m_pInputManager;
    std::int32_t field_3424;
    float field_3428;
    std::byte pad_342C[0x343D - 0x342C];
    bool m_bInputOn;
    bool m_bRecentringOn;
    bool m_bDrawCursor;
    std::byte pad_3440[0x4840 - 0x3440];
    std::int32_t field_4840;
    std::int32_t field_4844;
    bool m_bLMBDown;
    bool m_bMMBDown;
    bool m_bRMBDown;
    bool field_484B;
    std::byte pad_484C[0x4858 - 0x484C];
    std::int32_t field_4858;
    std::int32_t field_485C;
};

static_assert(offsetof(CMouseDX_Overlay, field_3408) == 0x3408);
static_assert(offsetof(CMouseDX_Overlay, field_341C) == 0x341C);
static_assert(offsetof(CMouseDX_Overlay, m_pInputManager) == 0x3420);
static_assert(offsetof(CMouseDX_Overlay, field_3428) == 0x3428);
static_assert(offsetof(CMouseDX_Overlay, m_bInputOn) == 0x343D);
static_assert(offsetof(CMouseDX_Overlay, m_bRecentringOn) == 0x343E);
static_assert(offsetof(CMouseDX_Overlay, m_bDrawCursor) == 0x343F);
static_assert(offsetof(CMouseDX_Overlay, field_4840) == 0x4840);
static_assert(offsetof(CMouseDX_Overlay, field_4844) == 0x4844);
static_assert(offsetof(CMouseDX_Overlay, m_bLMBDown) == 0x4848);
static_assert(offsetof(CMouseDX_Overlay, m_bMMBDown) == 0x4849);
static_assert(offsetof(CMouseDX_Overlay, m_bRMBDown) == 0x484A);
static_assert(offsetof(CMouseDX_Overlay, field_484B) == 0x484B);
static_assert(offsetof(CMouseDX_Overlay, field_4858) == 0x4858);
static_assert(offsetof(CMouseDX_Overlay, field_485C) == 0x485C);

using RawVFunc30 = void(__thiscall*)(void*, int);

// Raw thunk for the observed AddChildPrimitive call sequence.
// It preserves the live register inputs present at the constructor call site
// instead of asserting a recovered prototype for AddChildPrimitive itself.
extern std::uint32_t CallAddChildPrimitiveFromLiveState();

inline void CallVFunc30(void* object, int value)
{
    const auto vftable = *reinterpret_cast<void***>(object);
    const auto fn = reinterpret_cast<RawVFunc30>(vftable[0x30 / sizeof(void*)]);
    fn(object, value);
}
}

CMouseDX::CMouseDX(CInputManagerDX& inputManager, bool exclusive)
{
    NHeroInformationScreens_Mouse_Constructor(this, inputManager, exclusive);

    auto& self = *reinterpret_cast<CMouseDX_Overlay*>(this);
    self.vftable = &PTR__vector_deleting_destructor__0129ea14;
    self.m_pInputManager = &inputManager;
    self.field_4858 = 0;
    self.field_485C = 0;
    self.field_341C = 0;
    self.field_3428 = 1.0f;
    self.m_bInputOn = true;
    self.field_3408 = 0;
    CGameEventPackage::Clear(reinterpret_cast<CGameEventPackage*>(this));
    self.m_bRecentringOn = false;
    self.m_bDrawCursor = true;
    self.field_4840 = 0;
    self.field_4844 = 0;
    self.m_bLMBDown = false;
    self.m_bMMBDown = false;
    self.m_bRMBDown = false;
    self.field_484B = false;

    ::ShowCursor(static_cast<UINT>(!exclusive));

    const std::uint32_t childPrimitive = CallAddChildPrimitiveFromLiveState();
    auto* const object = *reinterpret_cast<void**>(
        *reinterpret_cast<std::uint32_t*>(childPrimitive + 0x60) + 0x58);
    CallVFunc30(object, 1);

    _SetExclusiveMode_CMouseDX__UAE_N_N_Z(this, exclusive);
}