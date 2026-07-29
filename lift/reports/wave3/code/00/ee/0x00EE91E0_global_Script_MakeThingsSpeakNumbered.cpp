void __fastcall Script_MakeThingsSpeakNumbered(CScriptBase* param_1)
{
    struct CScriptBase_40_Overlay
    {
        std::byte pad_00[0x40];
        void* field_40;
    };
    static_assert(offsetof(CScriptBase_40_Overlay, field_40) == 0x40);

    struct ReturnedHandleOverlay
    {
        void* vftable;
        int* field_04;
        int* field_08;
    };
    static_assert(offsetof(ReturnedHandleOverlay, field_04) == 0x04);
    static_assert(offsetof(ReturnedHandleOverlay, field_08) == 0x08);

    using NumberedTextPair = std::pair<EHeroMorphType, CParticleMorphs::CEntry>;

    auto vcall = []<typename Fn>(void* object, std::size_t byteOffset) -> Fn
    {
        return reinterpret_cast<Fn>((*reinterpret_cast<void***>(object))[byteOffset / sizeof(void*)]);
    };

    auto* const self40 = reinterpret_cast<CScriptBase_40_Overlay*>(param_1);

    using Fn_118_NoArgs = int(__thiscall*)(void*);
    auto* const returnedHandle =
        reinterpret_cast<ReturnedHandleOverlay*>(vcall.operator()<Fn_118_NoArgs>(self40->field_40, 0x118)(self40->field_40));

    void** ppuStack_28 = reinterpret_cast<void**>(&PTR__scalar_deleting_destructor__01238c8c);
    int* piStack_24 = returnedHandle->field_04;
    int* piStack_20 = returnedHandle->field_08;
    int* piStack_2c;
    unsigned long uStack_1c;
    int iStack_18;
    void** ppuStack_14;
    CScriptGameResourceObjectScriptedThingBase aCStack_10[4];
    void* uStack_c;
    void* uStack_8;
    std::uint32_t uStack_30;
    alignas(NumberedTextPair) std::byte stack0xffffffc8[sizeof(NumberedTextPair)];
    NumberedTextPair* extraout_EDX;
    NumberedTextPair* extraout_EDX_00;
    std::uint32_t unaff_EDI;

    if (piStack_20 != nullptr)
    {
        ++*piStack_20;
    }

    if (piStack_24 != nullptr)
    {
        using Fn_12C = char(__thiscall*)(int*);
        if (vcall.operator()<Fn_12C>(piStack_24, 0x12C)(piStack_24) != 0)
        {
            if (CScriptBase::IsActiveThreadTerminating(param_1))
            {
                C3DClothPrimitive::~C3DClothPrimitive(reinterpret_cast<C3DClothPrimitive*>(&ppuStack_28));
                return;
            }

            List_Node_Initialize(reinterpret_cast<int>(&uStack_30) + 3, reinterpret_cast<int>(&uStack_30) + 2);

            using Fn_950 = void(__thiscall*)(void*, unsigned long*);
            vcall.operator()<Fn_950>(self40->field_40, 0x950)(self40->field_40, &uStack_1c);

            if (iStack_18 != 0)
            {
                if (CScriptBase::IsActiveThreadTerminating(param_1))
                {
                    LTextTreeWalkThrough_dtor();
                    C3DClothPrimitive::~C3DClothPrimitive(reinterpret_cast<C3DClothPrimitive*>(&ppuStack_28));
                    return;
                }

                using Fn_5CC = void(__thiscall*)(void*, int);
                vcall.operator()<Fn_5CC>(self40->field_40, 0x5CC)(self40->field_40, 1);

                CBaseIntelligentPointer::CBaseIntelligentPointer(
                    reinterpret_cast<CBaseIntelligentPointer*>(&ppuStack_14));
                ppuStack_14 = reinterpret_cast<void**>(&PTR__scalar_deleting_destructor__0127094c);
                uStack_c = nullptr;
                uStack_8 = nullptr;

                using Fn_020 = void(__thiscall*)(void*, int**, void***, int);
                vcall.operator()<Fn_020>(self40->field_40, 0x20)(self40->field_40, &piStack_2c, &ppuStack_14, 4);

                unsigned long uVar6 = *reinterpret_cast<unsigned long*>(uStack_1c + 0x08);
                if (uVar6 != uStack_1c)
                {
                    do
                    {
                        if (CScriptBase::IsActiveThreadTerminating(param_1))
                        {
                            CScriptGameResourceObjectMovieBase::~CScriptGameResourceObjectMovieBase(
                                reinterpret_cast<CScriptGameResourceObjectMovieBase*>(aCStack_10));
                            CTCCarryable::OnKill(reinterpret_cast<CTCCarryable*>(&uStack_1c));
                            CFileInstaller::CActiveFile::OnReadFinished(
                                reinterpret_cast<CFileInstaller::CActiveFile*>(&uStack_1c));
                            C3DClothPrimitive::~C3DClothPrimitive(reinterpret_cast<C3DClothPrimitive*>(&ppuStack_28));
                            return;
                        }

                        const int iVar3 = **reinterpret_cast<int**>(self40->field_40);
                        const bool uVar10 = true;
                        CCharString* pCVar9 = nullptr;
                        auto* const ppVar4 =
                            reinterpret_cast<NumberedTextPair*>(GFIntToCharString_API());

                        reinterpret_cast<void(__thiscall*)(void*)>(iVar3 + 0x1C4)(self40->field_40);

                        std::_Cons_val<
                            std::allocator<NumberedTextPair>,
                            NumberedTextPair,
                            const NumberedTextPair&>(
                            reinterpret_cast<std::allocator<NumberedTextPair>*>(stack0xffffffc8),
                            extraout_EDX,
                            ppVar4);

                        CCharString* pCVar8 = nullptr;
                        char* pcVar7 = reinterpret_cast<char*>(1);

                        using Fn_118_WithArgs = CScriptThing*(__thiscall*)(void*, std::uint32_t, int, int);
                        auto* const pCVar5 =
                            vcall.operator()<Fn_118_WithArgs>(self40->field_40, 0x118)(
                                self40->field_40,
                                *reinterpret_cast<std::uint32_t*>(uVar6 + 0x10),
                                0,
                                0);

                        CScriptGameResourceObjectScriptedThingBase::
                            _Speak_CScriptGameResourceObjectScriptedThingBase__UAEXABVCScriptThing__KW4ETextGroupSelectionMethod___N22_Z(
                                aCStack_10,
                                pCVar5,
                                pcVar7,
                                pCVar8,
                                reinterpret_cast<CCharString*>(ppVar4),
                                pCVar9,
                                uVar10,
                                static_cast<bool>(static_cast<unsigned char>(unaff_EDI)));

                        if (CScriptThing::_IsPerformingScriptTask_CScriptGameResourceObjectScriptedThingBase__UBE_NXZ(
                                reinterpret_cast<CScriptThing*>(aCStack_10)))
                        {
                            do
                            {
                                using Fn_01C = void(__thiscall*)(void*);
                                vcall.operator()<Fn_01C>(self40->field_40, 0x1C)(self40->field_40);

                                if (CScriptBase::IsActiveThreadTerminating(param_1))
                                {
                                    CScriptGameResourceObjectMovieBase::~CScriptGameResourceObjectMovieBase(
                                        reinterpret_cast<CScriptGameResourceObjectMovieBase*>(aCStack_10));
                                    CTCCarryable::OnKill(reinterpret_cast<CTCCarryable*>(&uStack_1c));
                                    CFileInstaller::CActiveFile::OnReadFinished(
                                        reinterpret_cast<CFileInstaller::CActiveFile*>(&uStack_1c));
                                    C3DClothPrimitive::~C3DClothPrimitive(
                                        reinterpret_cast<C3DClothPrimitive*>(&ppuStack_28));
                                    return;
                                }
                            } while (CScriptThing::_IsPerformingScriptTask_CScriptGameResourceObjectScriptedThingBase__UBE_NXZ(
                                reinterpret_cast<CScriptThing*>(aCStack_10)));
                        }

                        if (CScriptBase::IsActiveThreadTerminating(param_1))
                        {
                            CScriptGameResourceObjectMovieBase::~CScriptGameResourceObjectMovieBase(
                                reinterpret_cast<CScriptGameResourceObjectMovieBase*>(aCStack_10));
                            CTCCarryable::OnKill(reinterpret_cast<CTCCarryable*>(&uStack_1c));
                            CFileInstaller::CActiveFile::OnReadFinished(
                                reinterpret_cast<CFileInstaller::CActiveFile*>(&uStack_1c));
                            C3DClothPrimitive::~C3DClothPrimitive(reinterpret_cast<C3DClothPrimitive*>(&ppuStack_28));
                            return;
                        }

                        uVar6 = CMemoryAllocatorVariableSize::GetNoAllocatedAreas(
                            reinterpret_cast<CMemoryAllocatorVariableSize*>(uVar6));
                    } while (uVar6 != uStack_1c);
                }

                if (CScriptBase::IsActiveThreadTerminating(param_1))
                {
                    CScriptGameResourceObjectMovieBase::~CScriptGameResourceObjectMovieBase(
                        reinterpret_cast<CScriptGameResourceObjectMovieBase*>(aCStack_10));
                    CTCCarryable::OnKill(reinterpret_cast<CTCCarryable*>(&uStack_1c));
                    CFileInstaller::CActiveFile::OnReadFinished(
                        reinterpret_cast<CFileInstaller::CActiveFile*>(&uStack_1c));
                    C3DClothPrimitive::~C3DClothPrimitive(reinterpret_cast<C3DClothPrimitive*>(&ppuStack_28));
                    return;
                }

                vcall.operator()<Fn_5CC>(self40->field_40, 0x5CC)(self40->field_40, 0);
                CScriptGameResourceObjectMovieBase::~CScriptGameResourceObjectMovieBase(
                    reinterpret_cast<CScriptGameResourceObjectMovieBase*>(aCStack_10));
            }

            LTextTreeWalkThrough_Cleanup();
        }
    }

    const int iVar3 = **reinterpret_cast<int**>(self40->field_40);

    using Fn_A3C = NumberedTextPair*(__thiscall*)(void*, int**, int);
    auto* const ppVar4 =
        reinterpret_cast<Fn_A3C>(iVar3 + 0xA3C)(self40->field_40, &piStack_2c, 0);

    reinterpret_cast<void(__thiscall*)(void*)>(iVar3 + 0x464)(self40->field_40);

    std::_Cons_val<
        std::allocator<NumberedTextPair>,
        NumberedTextPair,
        const NumberedTextPair&>(
        reinterpret_cast<std::allocator<NumberedTextPair>*>(stack0xffffffc8),
        extraout_EDX_00,
        ppVar4);

    if (piStack_2c != nullptr)
    {
        --*piStack_2c;
        if (*piStack_2c == 0)
        {
            reinterpret_cast<void(__thiscall*)(int*)>(piStack_2c[1])(piStack_2c);
            operator_delete(piStack_2c);
        }
    }

    uStack_30 = 0;
    piStack_2c = nullptr;
    NHeroInformationScreens::CBase::CBase(reinterpret_cast<CBase*>(&uStack_30));
}