#include <cstddef>
#include <cstdint>
#include <utility>

namespace
{
    template <typename T>
    [[nodiscard]] inline T &OffsetRef(void *base, std::size_t offset)
    {
        return *reinterpret_cast<T *>(static_cast<std::byte *>(base) + offset);
    }

    template <typename Ret, typename... Args>
    [[nodiscard]] inline Ret CallVFunc(void *object, std::size_t byteOffset, Args... args)
    {
        auto **const vftable = *reinterpret_cast<void ***>(object);
        auto *const fnSlot = reinterpret_cast<std::byte *>(vftable) + byteOffset;
        auto *const fn = *reinterpret_cast<Ret(__thiscall **)(void *, Args...)>(fnSlot);
        return fn(object, args...);
    }

    template <typename... Args>
    inline void CallVFuncVoid(void *object, std::size_t byteOffset, Args... args)
    {
        auto **const vftable = *reinterpret_cast<void ***>(object);
        auto *const fnSlot = reinterpret_cast<std::byte *>(vftable) + byteOffset;
        auto *const fn = *reinterpret_cast<void(__thiscall **)(void *, Args...)>(fnSlot);
        fn(object, args...);
    }

    template <typename T>
    [[nodiscard]] inline std::uint8_t SUB41(T value, int)
    {
        return static_cast<std::uint8_t>(static_cast<std::uint32_t>(value));
    }

    using ParticleMorphPair = std::pair<EHeroMorphType, CParticleMorphs::CEntry>;

    struct ScopedRefArea
    {
        void **ppuStack_28;
        int *piStack_24;
        int *piStack_20;
    };
    static_assert(offsetof(ScopedRefArea, ppuStack_28) == 0x0);
    static_assert(offsetof(ScopedRefArea, piStack_24) == 0x4);
    static_assert(offsetof(ScopedRefArea, piStack_20) == 0x8);

    struct UStack30Area
    {
        std::uint32_t uStack_30;
        int *piStack_2c;
    };
    static_assert(offsetof(UStack30Area, uStack_30) == 0x0);
    static_assert(offsetof(UStack30Area, piStack_2c) == 0x4);

    struct WalkArea
    {
        std::uintptr_t uStack_1c;
        int iStack_18;
    };
    static_assert(offsetof(WalkArea, uStack_1c) == 0x0);
    static_assert(offsetof(WalkArea, iStack_18) == 0x4);

    struct IntelligentPointerArea
    {
        void **ppuStack_14;
        std::uint32_t uStack_c;
        std::uint32_t uStack_8;
    };
    static_assert(offsetof(IntelligentPointerArea, ppuStack_14) == 0x0);
    static_assert(offsetof(IntelligentPointerArea, uStack_c) == 0x4);
    static_assert(offsetof(IntelligentPointerArea, uStack_8) == 0x8);

    struct ScriptedThingStackArea
    {
        std::byte bytes[0x10];
    };
    static_assert(sizeof(ScriptedThingStackArea) == 0x10);

    struct ConsValStackArea
    {
        std::byte bytes[sizeof(ParticleMorphPair)];
    };
    static_assert(sizeof(ConsValStackArea) == sizeof(ParticleMorphPair));
}

void __fastcall Script_MakeThingsSpeakNumbered(CScriptBase *param_1)
{
    char cVar1;
    bool bVar2;
    int iVar3;
    ParticleMorphPair *ppVar4;
    CScriptThing *pCVar5;
    std::uintptr_t uVar6;
    CMemoryAllocatorVariableSize *this_;
    ParticleMorphPair *extraout_EDX;
    ParticleMorphPair *extraout_EDX_00;
    std::uint32_t unaff_EDI;
    char *pcVar7;
    CCharString *pCVar8;
    CCharString *pCVar9;
    std::uint8_t uVar10;
    UStack30Area uStack30Area{};
    ScopedRefArea scopedRef{};
    WalkArea walk{};
    IntelligentPointerArea intelligentPointer{};
    ScriptedThingStackArea aCStack_10{};
    ConsValStackArea stack0xffffffc8{};

    void *const scriptContext = OffsetRef<void *>(param_1, 0x40);

    iVar3 = CallVFunc<int>(scriptContext, 0x118);
    scopedRef.ppuStack_28 = reinterpret_cast<void **>(&PTR__scalar_deleting_destructor__01238c8c);
    scopedRef.piStack_24 = *reinterpret_cast<int **>(iVar3 + 4);
    scopedRef.piStack_20 = *reinterpret_cast<int **>(iVar3 + 8);
    if (scopedRef.piStack_20 != nullptr) {
        *scopedRef.piStack_20 = *scopedRef.piStack_20 + 1;
    }

    if ((scopedRef.piStack_24 != nullptr) &&
        ((cVar1 = CallVFunc<char>(scopedRef.piStack_24, 300)), cVar1 != '\0')) {
        bVar2 = CScriptBase::IsActiveThreadTerminating(param_1);
        if (bVar2) {
LAB_00EE93A8:
            C3DClothPrimitive::~C3DClothPrimitive(reinterpret_cast<C3DClothPrimitive *>(&scopedRef));
            return;
        }

        List_Node_Initialize(
            reinterpret_cast<int>(&uStack30Area.uStack_30) + 3,
            reinterpret_cast<int>(&uStack30Area.uStack_30) + 2);

        CallVFuncVoid(scriptContext, 0x950, &walk.uStack_1c);
        if (walk.iStack_18 != 0) {
            bVar2 = CScriptBase::IsActiveThreadTerminating(param_1);
            if (bVar2) {
                LTextTreeWalkThrough_dtor();
                C3DClothPrimitive::~C3DClothPrimitive(reinterpret_cast<C3DClothPrimitive *>(&scopedRef));
                return;
            }

            CallVFuncVoid(scriptContext, 0x5CC, 1);

            CBaseIntelligentPointer::CBaseIntelligentPointer(
                reinterpret_cast<CBaseIntelligentPointer *>(&intelligentPointer));
            intelligentPointer.ppuStack_14 =
                reinterpret_cast<void **>(&PTR__scalar_deleting_destructor__0127094c);
            intelligentPointer.uStack_c = 0;
            intelligentPointer.uStack_8 = 0;

            CallVFuncVoid(scriptContext, 0x20, &uStack30Area.piStack_2c, &intelligentPointer, 4);

            uVar6 = *reinterpret_cast<std::uintptr_t *>(walk.uStack_1c + 8);
            if (uVar6 != walk.uStack_1c) {
                do {
                    bVar2 = CScriptBase::IsActiveThreadTerminating(param_1);
                    if (bVar2) {
                        goto LAB_00EE9391;
                    }

                    uVar10 = 1;
                    pCVar9 = nullptr;
                    ppVar4 = reinterpret_cast<ParticleMorphPair *>(GFIntToCharString_API());

                    CallVFuncVoid(scriptContext, 0x1C4);

                    std::_Cons_val<
                        std::allocator<ParticleMorphPair>,
                        ParticleMorphPair,
                        const ParticleMorphPair &
                    >(
                        reinterpret_cast<std::allocator<ParticleMorphPair> *>(&stack0xffffffc8),
                        extraout_EDX,
                        ppVar4);

                    pCVar8 = nullptr;
                    pcVar7 = reinterpret_cast<char *>(1);

                    pCVar5 = reinterpret_cast<CScriptThing *>(
                        CallVFunc<void *>(
                            scriptContext,
                            0x118,
                            *reinterpret_cast<std::uint32_t *>(uVar6 + 0x10),
                            0,
                            0));

                    CScriptGameResourceObjectScriptedThingBase::
                    _Speak_CScriptGameResourceObjectScriptedThingBase__UAEXABVCScriptThing__KW4ETextGroupSelectionMethod___N22_Z(
                        reinterpret_cast<CScriptGameResourceObjectScriptedThingBase *>(&aCStack_10),
                        pCVar5,
                        pcVar7,
                        pCVar8,
                        reinterpret_cast<CCharString *>(ppVar4),
                        pCVar9,
                        static_cast<bool>(uVar10),
                        SUB41(unaff_EDI, 0));

                    bVar2 =
                        CScriptThing::_IsPerformingScriptTask_CScriptGameResourceObjectScriptedThingBase__UBE_NXZ(
                            reinterpret_cast<CScriptThing *>(&aCStack_10));
                    if (bVar2) {
                        do {
                            CallVFuncVoid(scriptContext, 0x1C);
                            bVar2 = CScriptBase::IsActiveThreadTerminating(param_1);
                            if (bVar2) {
                                goto LAB_00EE9391;
                            }
                            bVar2 =
                                CScriptThing::_IsPerformingScriptTask_CScriptGameResourceObjectScriptedThingBase__UBE_NXZ(
                                    reinterpret_cast<CScriptThing *>(&aCStack_10));
                        } while (bVar2);
                    }

                    bVar2 = CScriptBase::IsActiveThreadTerminating(param_1);
                    if (bVar2) {
                        goto LAB_00EE9391;
                    }

                    this_ = reinterpret_cast<CMemoryAllocatorVariableSize *>(uVar6);
                    uVar6 = CMemoryAllocatorVariableSize::GetNoAllocatedAreas(this_);
                } while (uVar6 != walk.uStack_1c);
            }

            bVar2 = CScriptBase::IsActiveThreadTerminating(param_1);
            if (bVar2) {
LAB_00EE9391:
                CScriptGameResourceObjectMovieBase::~CScriptGameResourceObjectMovieBase(
                    reinterpret_cast<CScriptGameResourceObjectMovieBase *>(&aCStack_10));
                CTCCarryable::OnKill(reinterpret_cast<CTCCarryable *>(&walk.uStack_1c));
                CFileInstaller::CActiveFile::OnReadFinished(
                    reinterpret_cast<CFileInstaller::CActiveFile *>(&walk.uStack_1c));
                goto LAB_00EE93A8;
            }

            CallVFuncVoid(scriptContext, 0x5CC, 0);
            CScriptGameResourceObjectMovieBase::~CScriptGameResourceObjectMovieBase(
                reinterpret_cast<CScriptGameResourceObjectMovieBase *>(&aCStack_10));
        }

        LTextTreeWalkThrough_Cleanup();
    }

    iVar3 = *reinterpret_cast<int *>(scriptContext);
    ppVar4 = reinterpret_cast<ParticleMorphPair *>(
        CallVFunc<void *>(scriptContext, 0xA3C, &uStack30Area.piStack_2c, 0));

    CallVFuncVoid(scriptContext, 0x464);

    std::_Cons_val<
        std::allocator<ParticleMorphPair>,
        ParticleMorphPair,
        const ParticleMorphPair &
    >(
        reinterpret_cast<std::allocator<ParticleMorphPair> *>(&stack0xffffffc8),
        extraout_EDX_00,
        ppVar4);

    if ((uStack30Area.piStack_2c != nullptr) &&
        (*uStack30Area.piStack_2c = *uStack30Area.piStack_2c + -1, *uStack30Area.piStack_2c == 0)) {
        (*reinterpret_cast<void (**)()>(uStack30Area.piStack_2c + 1))();
        operator_delete(uStack30Area.piStack_2c);
    }

    uStack30Area.uStack_30 = 0;
    uStack30Area.piStack_2c = nullptr;
    NHeroInformationScreens::CBase::CBase(reinterpret_cast<CBase *>(&uStack30Area));
}