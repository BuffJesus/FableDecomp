#include <cstddef>
#include <cstdint>

struct CEnginePrimitive;
struct CDisplayManager;

struct CEngineInternalPrimitiveBase
{
    CEngineInternalPrimitiveBase* __thiscall AddChildPrimitive(unsigned long param_2);
};

struct CXMVPlayer
{
    static void GlobalRegisterPlayer();
};

#pragma pack(push, 1)
struct CEngineInternalPrimitiveBase_Overlay
{
    std::byte pad00[0x3AB0];
    std::uint32_t field_3AB0; // 0x3AB0
    std::uint32_t field_3AB4; // 0x3AB4
};

struct CDisplayManager_Overlay
{
    std::byte pad00[0x08];
    CEngineInternalPrimitiveBase* field_08; // 0x08
    std::byte pad0C[0x1BC];
    std::int32_t field_1C8; // 0x1C8
};

struct ChildPrimitive_7C_Overlay
{
    std::byte pad00[0x7C];
    std::uint32_t field_7C; // 0x7C
};

struct ChildPrimitive_60_Overlay
{
    std::byte pad00[0x60];
    CDisplayManager* field_60; // 0x60
};

struct ChildPrimitive_58_Overlay
{
    std::byte pad00[0x58];
    std::uint32_t field_58; // 0x58
};

struct ChildPrimitive_84_Overlay
{
    std::byte pad00[0x84];
    std::uint32_t field_84; // 0x84
};

struct ChildPrimitive_6C_Overlay
{
    std::byte pad00[0x6C];
    std::uint32_t field_6C; // 0x6C
};
#pragma pack(pop)

static_assert(offsetof(CEngineInternalPrimitiveBase_Overlay, field_3AB0) == 0x3AB0);
static_assert(offsetof(CEngineInternalPrimitiveBase_Overlay, field_3AB4) == 0x3AB4);
static_assert(offsetof(CDisplayManager_Overlay, field_08) == 0x08);
static_assert(offsetof(CDisplayManager_Overlay, field_1C8) == 0x1C8);
static_assert(offsetof(ChildPrimitive_7C_Overlay, field_7C) == 0x7C);
static_assert(offsetof(ChildPrimitive_60_Overlay, field_60) == 0x60);
static_assert(offsetof(ChildPrimitive_58_Overlay, field_58) == 0x58);
static_assert(offsetof(ChildPrimitive_84_Overlay, field_84) == 0x84);
static_assert(offsetof(ChildPrimitive_6C_Overlay, field_6C) == 0x6C);

extern std::uint32_t DAT_013b8394;
extern CDisplayManager* DAT_013b8390;
extern CEngineInternalPrimitiveBase* DAT_013b8384;
extern void* _DAT_013b8380;
extern std::byte DAT_013bc470;
extern std::uint32_t DAT_013b8388;
extern std::uint32_t DAT_013b838c;
extern std::uint32_t DAT_013b837c;
extern std::uint32_t DAT_013b7d68;
extern std::uint32_t DAT_01375470;
extern std::uint32_t DAT_0137546c;
extern std::uint32_t DAT_013b7d64;
extern std::uint32_t DAT_013b7d60;
extern std::int32_t _DAT_0137544c;

extern bool __thiscall CDisplayManager::IsTextureBitDepthSupported(unsigned long bitDepth);
extern void GFGetBuildNumber2();
extern std::uint32_t GFInitialise_SetupProgressDisplay();

std::uint32_t __thiscall GFInitialise(CEngineInternalPrimitiveBase* param_1, unsigned long param_2)
{
    auto* const child7C =
        reinterpret_cast<ChildPrimitive_7C_Overlay*>(param_1->AddChildPrimitive(param_2));
    DAT_013b8394 = child7C->field_7C;

    auto* const child60 =
        reinterpret_cast<ChildPrimitive_60_Overlay*>(param_1->AddChildPrimitive(param_2));
    DAT_013b8390 = child60->field_60;
    DAT_013b8384 = reinterpret_cast<CDisplayManager_Overlay*>(DAT_013b8390)->field_08;
    _DAT_013b8380 = &DAT_013bc470;

    auto* const child58 =
        reinterpret_cast<ChildPrimitive_58_Overlay*>(DAT_013b8384->AddChildPrimitive(param_2));
    DAT_013b8388 = child58->field_58;

    auto* const child84 =
        reinterpret_cast<ChildPrimitive_84_Overlay*>(DAT_013b8384->AddChildPrimitive(param_2));
    DAT_013b838c = child84->field_84;

    auto* const child6C =
        reinterpret_cast<ChildPrimitive_6C_Overlay*>(DAT_013b8384->AddChildPrimitive(param_2));
    DAT_013b837c = child6C->field_6C;

    DAT_013b7d68 = DAT_01375470;
    if (DAT_013b8390->IsTextureBitDepthSupported(DAT_01375470)) {
        DAT_013b7d68 = DAT_01375470;
    } else if (DAT_013b8390->IsTextureBitDepthSupported(0x10)) {
        DAT_013b7d68 = 0x10;
    } else if (DAT_013b8390->IsTextureBitDepthSupported(0x18)) {
        DAT_013b7d68 = 0x18;
    } else if (DAT_013b8390->IsTextureBitDepthSupported(0x20)) {
        DAT_013b7d68 = 0x20;
    }

    auto* const primitiveBase =
        reinterpret_cast<CEngineInternalPrimitiveBase_Overlay*>(DAT_013b8384);

    DAT_013b7d64 = primitiveBase->field_3AB0;
    if (DAT_0137546c < primitiveBase->field_3AB0) {
        DAT_013b7d64 = DAT_0137546c;
    }

    DAT_013b7d60 = primitiveBase->field_3AB4;
    if (DAT_0137546c < primitiveBase->field_3AB4) {
        DAT_013b7d60 = DAT_0137546c;
    }

    CXMVPlayer::GlobalRegisterPlayer();
    GFGetBuildNumber2();

    const std::uint32_t setupResult = GFInitialise_SetupProgressDisplay();
    if (static_cast<std::uint8_t>(setupResult) == 0x01) {
        const std::int32_t halfWidth =
            reinterpret_cast<CDisplayManager_Overlay*>(DAT_013b8390)->field_1C8 / 2;
        _DAT_0137544c = halfWidth;
        return (static_cast<std::uint32_t>(halfWidth) & 0xFFFFFF00u) | 0x01u;
    }

    return setupResult & 0xFFFFFF00u;
}