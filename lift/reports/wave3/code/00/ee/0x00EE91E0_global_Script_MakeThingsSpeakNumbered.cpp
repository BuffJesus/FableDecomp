#include <cstddef>
#include <cstdint>
#include <memory>
#include <utility>

static_assert(sizeof(void*) == 4, "This reversal models a 32-bit stack frame.");

struct CScriptBase;
struct CScriptThing;
struct CCharString;
struct CBase;
struct C3DClothPrimitive;
struct CBaseIntelligentPointer;
struct CScriptGameResourceObjectMovieBase;
struct CScriptGameResourceObjectScriptedThingBase;
struct CMemoryAllocatorVariableSize;
struct CTCCarryable;

namespace CFileInstaller {
struct CActiveFile;
}

enum EHeroMorphType : int;
struct CParticleMorphs {
    struct CEntry;
};

using ParticleMorphPair = std::pair<EHeroMorphType, CParticleMorphs::CEntry>;
using ParticleMorphPairAllocator = std::allocator<ParticleMorphPair>;

extern bool CScriptBase::IsActiveThreadTerminating(CScriptBase*);

extern void List_Node_Initialize(int, int);
extern void LTextTreeWalkThrough_dtor();
extern void LTextTreeWalkThrough_Cleanup();

extern void C3DClothPrimitive::~C3DClothPrimitive(C3DClothPrimitive*);
extern void CBaseIntelligentPointer::CBaseIntelligentPointer(CBaseIntelligentPointer*);
extern void CScriptGameResourceObjectMovieBase::~CScriptGameResourceObjectMovieBase(CScriptGameResourceObjectMovieBase*);
extern void CTCCarryable::OnKill(CTCCarryable*);
extern void CFileInstaller::CActiveFile::OnReadFinished(CFileInstaller::CActiveFile*);
extern std::uint32_t CMemoryAllocatorVariableSize::GetNoAllocatedAreas(CMemoryAllocatorVariableSize*);
extern ParticleMorphPair* GFIntToCharString_API();
extern void StdConsVal_ParticleMorphPair(
    ParticleMorphPairAllocator* dst,
    ParticleMorphPair* extraout_edx,
    const ParticleMorphPair* value);
extern void operator_delete(void*);
extern void NHeroInformationScreens::CBase::CBase(CBase*);

extern bool CScriptThing::_IsPerformingScriptTask_CScriptGameResourceObjectScriptedThingBase__UBE_NXZ(CScriptThing*);
extern void CScriptGameResourceObjectScriptedThingBase::_Speak_CScriptGameResourceObjectScriptedThingBase__UAEXABVCScriptThing__KW4ETextGroupSelectionMethod___N22_Z(
    CScriptGameResourceObjectScriptedThingBase*,
    CScriptThing*,
    char*,
    CCharString*,
    CCharString*,
    CCharString*,
    bool,
    bool);

extern ParticleMorphPair* GFIntToCharString_API_WithExtraoutEDX(ParticleMorphPair** extraout_edx);
extern ParticleMorphPair* Object40_VFuncA3C_WithExtraoutEDX(
    void* object_40,
    int** piStack_2c,
    int zero,
    ParticleMorphPair** extraout_edx);

template <typename T>
static T VCall(void* object, std::size_t byte_offset) {
    return reinterpret_cast<T>((*reinterpret_cast<void***>(object))[byte_offset / 4]);
}

struct ScriptBaseOverlay {
    std::byte pad_00[0x40];
    void* object_40;
};
static_assert(offsetof(ScriptBaseOverlay, object_40) == 0x40);

struct Acquire118ResultOverlay {
    void* unk_00;
    int* ptr_04;
    int* ref_08;
};
static_assert(offsetof(Acquire118ResultOverlay, ptr_04) == 0x04);
static_assert(offsetof(Acquire118ResultOverlay, ref_08) == 0x08);

struct SpeakTaskLocal {
    std::byte storage[4];
};
static_assert(sizeof(SpeakTaskLocal) == 4);

struct FunctionStackOverlay {
    ParticleMorphPairAllocator cons_val_slot_c8;
    std::uint32_t uStack_30;
    int* piStack_2c;
    void* ppuStack_28;
    int* piStack_24;
    int* piStack_20;
    std::uint32_t uStack_1c;
    int iStack_18;
    void* ppuStack_14;
    SpeakTaskLocal aCStack_10;
    std::uint32_t uStack_c;
    std::uint32_t uStack_8;
};
static_assert(offsetof(FunctionStackOverlay, cons_val_slot_c8) == 0x00);
static_assert(offsetof(FunctionStackOverlay, uStack_30) == 0x04);
static_assert(offsetof(FunctionStackOverlay, piStack_2c) == 0x08);
static_assert(offsetof(FunctionStackOverlay, ppuStack_28) == 0x0C);
static_assert(offsetof(FunctionStackOverlay, piStack_24) == 0x10);
static_assert(offsetof(FunctionStackOverlay, piStack_20) == 0x14);
static_assert(offsetof(FunctionStackOverlay, uStack_1c) == 0x18);
static_assert(offsetof(FunctionStackOverlay, iStack_18) == 0x1C);
static_assert(offsetof(FunctionStackOverlay, ppuStack_14) == 0x20);
static_assert(offsetof(FunctionStackOverlay, aCStack_10) == 0x24);
static_assert(offsetof(FunctionStackOverlay, uStack_c) == 0x28);
static_assert(offsetof(FunctionStackOverlay, uStack_8) == 0x2C);

void __fastcall Script_MakeThingsSpeakNumbered(CScriptBase* param_1) {
    FunctionStackOverlay frame{};
    auto* const object_40 = reinterpret_cast<ScriptBaseOverlay*>(param_1)->object_40;

    using Fn118NoArgs = std::intptr_t(__thiscall*)(void*);
    const auto iVar3 = VCall<Fn118NoArgs>(object_40, 0x118)(object_40);

    frame.ppuStack_28 = reinterpret_cast<void*>(0x01238C8C);
    frame.piStack_24 = reinterpret_cast<Acquire118ResultOverlay*>(iVar3)->ptr_04;
    frame.piStack_20 = reinterpret_cast<Acquire118ResultOverlay*>(iVar3)->ref_08;

    if (frame.piStack_20 != nullptr) {
        *frame.piStack_20 = *frame.piStack_20 + 1;
    }

    if (frame.piStack_24 != nullptr) {
        using Fn300 = char(__thiscall*)(int*);
        if (VCall<Fn300>(frame.piStack_24, 300)(frame.piStack_24) != '\0') {
            if (CScriptBase::IsActiveThreadTerminating(param_1)) {
LAB_00EE93A8:
                C3DClothPrimitive::~C3DClothPrimitive(
                    reinterpret_cast<C3DClothPrimitive*>(&frame.ppuStack_28));
                return;
            }

            List_Node_Initialize(
                static_cast<int>(reinterpret_cast<std::uintptr_t>(&frame.uStack_30) + 3),
                static_cast<int>(reinterpret_cast<std::uintptr_t>(&frame.uStack_30) + 2));

            using Fn950 = void(__thiscall*)(void*, void*);
            VCall<Fn950>(object_40, 0x950)(object_40, &frame.uStack_1c);

            if (frame.iStack_18 != 0) {
                if (CScriptBase::IsActiveThreadTerminating(param_1)) {
                    LTextTreeWalkThrough_dtor();
                    C3DClothPrimitive::~C3DClothPrimitive(
                        reinterpret_cast<C3DClothPrimitive*>(&frame.ppuStack_28));
                    return;
                }

                using Fn5CC = void(__thiscall*)(void*, int);
                VCall<Fn5CC>(object_40, 0x5CC)(object_40, 1);

                CBaseIntelligentPointer::CBaseIntelligentPointer(
                    reinterpret_cast<CBaseIntelligentPointer*>(&frame.ppuStack_14));
                frame.ppuStack_14 = reinterpret_cast<void*>(0x0127094C);
                frame.uStack_c = 0;
                frame.uStack_8 = 0;

                using Fn020 = void(__thiscall*)(void*, int**, void*, int);
                VCall<Fn020>(object_40, 0x20)(object_40, &frame.piStack_2c, &frame.ppuStack_14, 4);

                std::uint32_t uVar6 = *reinterpret_cast<std::uint32_t*>(frame.uStack_1c + 8);
                if (uVar6 != frame.uStack_1c) {
                    do {
                        if (CScriptBase::IsActiveThreadTerminating(param_1)) {
LAB_00EE9391:
                            CScriptGameResourceObjectMovieBase::~CScriptGameResourceObjectMovieBase(
                                reinterpret_cast<CScriptGameResourceObjectMovieBase*>(&frame.aCStack_10));
                            CTCCarryable::OnKill(reinterpret_cast<CTCCarryable*>(&frame.uStack_1c));
                            CFileInstaller::CActiveFile::OnReadFinished(
                                reinterpret_cast<CFileInstaller::CActiveFile*>(&frame.uStack_1c));
                            goto LAB_00EE93A8;
                        }

                        std::uint32_t iVar3_vftable = **reinterpret_cast<std::uint32_t**>(object_40);

                        ParticleMorphPair* extraout_EDX = nullptr;
                        ParticleMorphPair* const ppVar4 =
                            GFIntToCharString_API_WithExtraoutEDX(&extraout_EDX);

                        reinterpret_cast<void(__thiscall*)(void*)>(
                            *reinterpret_cast<void**>(iVar3_vftable + 0x1C4))(object_40);

                        StdConsVal_ParticleMorphPair(&frame.cons_val_slot_c8, extraout_EDX, ppVar4);

                        using Fn118WithArgs = CScriptThing*(__thiscall*)(void*, std::uint32_t, int, int);
                        CScriptThing* const pCVar5 =
                            VCall<Fn118WithArgs>(object_40, 0x118)(
                                object_40,
                                *reinterpret_cast<std::uint32_t*>(uVar6 + 0x10),
                                0,
                                0);

                        CCharString* const pCVar8 = nullptr;
                        CCharString* const pCVar9 = nullptr;
                        char* const pcVar7 = reinterpret_cast<char*>(1);
                        const bool uVar10 = true;
                        std::uint32_t unaff_EDI;
                        CScriptGameResourceObjectScriptedThingBase::_Speak_CScriptGameResourceObjectScriptedThingBase__UAEXABVCScriptThing__KW4ETextGroupSelectionMethod___N22_Z(
                            reinterpret_cast<CScriptGameResourceObjectScriptedThingBase*>(&frame.aCStack_10),
                            pCVar5,
                            pcVar7,
                            pCVar8,
                            reinterpret_cast<CCharString*>(ppVar4),
                            pCVar9,
                            uVar10,
                            static_cast<bool>(static_cast<std::uint8_t>(unaff_EDI)));

                        if (CScriptThing::_IsPerformingScriptTask_CScriptGameResourceObjectScriptedThingBase__UBE_NXZ(
                                reinterpret_cast<CScriptThing*>(&frame.aCStack_10))) {
                            do {
                                using Fn01C = void(__thiscall*)(void*);
                                VCall<Fn01C>(object_40, 0x1C)(object_40);

                                if (CScriptBase::IsActiveThreadTerminating(param_1)) {
                                    goto LAB_00EE9391;
                                }
                            } while (CScriptThing::_IsPerformingScriptTask_CScriptGameResourceObjectScriptedThingBase__UBE_NXZ(
                                reinterpret_cast<CScriptThing*>(&frame.aCStack_10)));
                        }

                        if (CScriptBase::IsActiveThreadTerminating(param_1)) {
                            goto LAB_00EE9391;
                        }

                        CMemoryAllocatorVariableSize* const this_ =
                            reinterpret_cast<CMemoryAllocatorVariableSize*>(uVar6);
                        uVar6 = CMemoryAllocatorVariableSize::GetNoAllocatedAreas(this_);
                    } while (uVar6 != frame.uStack_1c);
                }

                if (CScriptBase::IsActiveThreadTerminating(param_1)) {
                    goto LAB_00EE9391;
                }

                VCall<Fn5CC>(object_40, 0x5CC)(object_40, 0);
                CScriptGameResourceObjectMovieBase::~CScriptGameResourceObjectMovieBase(
                    reinterpret_cast<CScriptGameResourceObjectMovieBase*>(&frame.aCStack_10));
            }

            LTextTreeWalkThrough_Cleanup();
        }
    }

    frame.piStack_2c = nullptr;

    ParticleMorphPair* extraout_EDX_00 = nullptr;
    ParticleMorphPair* const ppVar4 =
        Object40_VFuncA3C_WithExtraoutEDX(object_40, &frame.piStack_2c, 0, &extraout_EDX_00);

    using Fn464 = void(__thiscall*)(void*);
    VCall<Fn464>(object_40, 0x464)(object_40);

    StdConsVal_ParticleMorphPair(&frame.cons_val_slot_c8, extraout_EDX_00, ppVar4);

    if (frame.piStack_2c != nullptr) {
        *frame.piStack_2c = *frame.piStack_2c + -1;
        if (*frame.piStack_2c == 0) {
            reinterpret_cast<void(__thiscall*)(int*)>(frame.piStack_2c[1])(frame.piStack_2c);
            operator_delete(frame.piStack_2c);
        }
    }

    frame.uStack_30 = 0;
    frame.piStack_2c = nullptr;
    NHeroInformationScreens::CBase::CBase(reinterpret_cast<CBase*>(&frame.uStack_30));
}