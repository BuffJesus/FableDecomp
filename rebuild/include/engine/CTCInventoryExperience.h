#ifndef FABLE_ENGINE_CTCINVENTORYEXPERIENCE_H
#define FABLE_ENGINE_CTCINVENTORYEXPERIENCE_H
// PDB-generated layout with parity-backed method declarations maintained here.
// Donor: Ego_r.pdb (ghidra_out/struct_layouts_egor.tsv) sizeof CTCInventoryExperience = 0x2d8.
// Retail evidence: 2 trusted landed function(s); reconciliation clean.
// Members are original Lionhead names; `_pad_*` are unnamed bytes; opaque
// `unsigned char X[N]` members carry their real (composite/template) type in a comment.
#include <stddef.h>
#include "rebuild_abi.h"

struct CComponent;
struct CViewport;
struct CYesNo;

#pragma pack(push, 1)
struct CTCInventoryExperience {
    void*         __vftable;                                 // +0x00 vptr, or first dword of a flattened base subobject
    unsigned char _pad_0x04[0x4];                            // +0x04
    unsigned char _pad_0x08[0x4];                            // +0x08
    unsigned char _pad_0x0c[0x4];                            // +0x0c
    unsigned char _pad_0x10[0x4];                            // +0x10
    unsigned char _pad_0x14[0x4];                            // +0x14
    unsigned char _pad_0x18[0x4];                            // +0x18
    unsigned char _pad_0x1c[0x4];                            // +0x1c
    unsigned char _pad_0x20[0x4];                            // +0x20
    unsigned char _pad_0x24[0x4];                            // +0x24
    unsigned char _pad_0x28[0x4];                            // +0x28
    unsigned char _pad_0x2c[0x4];                            // +0x2c
    unsigned char _pad_0x30[0x4];                            // +0x30
    unsigned char _pad_0x34[0x4];                            // +0x34
    unsigned char _pad_0x38[0x4];                            // +0x38
    unsigned char _pad_0x3c[0x4];                            // +0x3c
    unsigned char _pad_0x40[0x4];                            // +0x40
    unsigned char _pad_0x44[0x4];                            // +0x44
    unsigned char _pad_0x48[0x4];                            // +0x48
    unsigned char _pad_0x4c[0x4];                            // +0x4c
    unsigned char _pad_0x50[0x4];                            // +0x50
    unsigned char _pad_0x54[0x4];                            // +0x54
    unsigned char _pad_0x58[0x4];                            // +0x58
    unsigned char _pad_0x5c[0x4];                            // +0x5c
    unsigned char _pad_0x60[0x4];                            // +0x60
    unsigned char _pad_0x64[0x4];                            // +0x64
    unsigned char _pad_0x68[0x4];                            // +0x68
    unsigned char _pad_0x6c[0x4];                            // +0x6c
    unsigned char _pad_0x70[0x4];                            // +0x70
    unsigned char _pad_0x74[0x4];                            // +0x74
    unsigned char _pad_0x78[0x4];                            // +0x78
    unsigned char _pad_0x7c[0x4];                            // +0x7c
    unsigned char _pad_0x80[0x4];                            // +0x80
    unsigned char _pad_0x84[0x4];                            // +0x84
    unsigned char _pad_0x88[0x4];                            // +0x88
    unsigned char _pad_0x8c[0x4];                            // +0x8c
    unsigned char _pad_0x90[0x4];                            // +0x90
    unsigned char _pad_0x94[0x4];                            // +0x94
    unsigned char _pad_0x98[0x4];                            // +0x98
    unsigned char _pad_0x9c[0x4];                            // +0x9c
    unsigned char _pad_0xa0[0x4];                            // +0xa0
    unsigned char _pad_0xa4[0x4];                            // +0xa4
    unsigned char _pad_0xa8[0x4];                            // +0xa8
    unsigned char _pad_0xac[0x4];                            // +0xac
    unsigned char _pad_0xb0[0x4];                            // +0xb0
    unsigned char _pad_0xb4[0x4];                            // +0xb4
    unsigned char _pad_0xb8[0x4];                            // +0xb8
    unsigned char _pad_0xbc[0x4];                            // +0xbc
    unsigned char _pad_0xc0[0x4];                            // +0xc0
    unsigned char _pad_0xc4[0x4];                            // +0xc4
    unsigned char _pad_0xc8[0x4];                            // +0xc8
    unsigned char _pad_0xcc[0x4];                            // +0xcc
    unsigned char _pad_0xd0[0x4];                            // +0xd0
    unsigned char _pad_0xd4[0x4];                            // +0xd4
    unsigned char _pad_0xd8[0x4];                            // +0xd8
    unsigned char _pad_0xdc[0x4];                            // +0xdc
    unsigned char _pad_0xe0[0x4];                            // +0xe0
    unsigned char _pad_0xe4[0x4];                            // +0xe4
    unsigned char _pad_0xe8[0x4];                            // +0xe8
    unsigned char _pad_0xec[0x4];                            // +0xec
    unsigned char _pad_0xf0[0x4];                            // +0xf0
    unsigned char _pad_0xf4[0x4];                            // +0xf4
    unsigned char _pad_0xf8[0x4];                            // +0xf8
    unsigned char _pad_0xfc[0x4];                            // +0xfc
    unsigned char _pad_0x100[0x4];                           // +0x100
    unsigned char _pad_0x104[0x4];                           // +0x104
    unsigned char _pad_0x108[0x4];                           // +0x108
    unsigned char _pad_0x10c[0x4];                           // +0x10c
    unsigned char _pad_0x110[0x4];                           // +0x110
    unsigned char _pad_0x114[0x4];                           // +0x114
    unsigned char _pad_0x118[0x4];                           // +0x118
    unsigned char _pad_0x11c[0x4];                           // +0x11c
    unsigned char _pad_0x120[0x4];                           // +0x120
    unsigned char _pad_0x124[0x4];                           // +0x124
    unsigned char _pad_0x128[0x4];                           // +0x128
    unsigned char _pad_0x12c[0x4];                           // +0x12c
    unsigned char _pad_0x130[0x4];                           // +0x130
    unsigned char _pad_0x134[0x4];                           // +0x134
    unsigned char _pad_0x138[0x4];                           // +0x138
    unsigned char _pad_0x13c[0x4];                           // +0x13c
    unsigned char _pad_0x140[0x4];                           // +0x140
    unsigned char _pad_0x144[0x4];                           // +0x144
    unsigned char _pad_0x148[0x4];                           // +0x148
    unsigned char TutorialSoundHandles[0x10];                // +0x14c map<CCharString,unsigned_long,std::less<CCharString>,std::allocator<std::pair<CCharString_const_,unsigned_long>_>_>
    unsigned char UndoableStatBuys[0x10];                    // +0x15c map<EHeroTrainableStatType,SExpSpendUndo,std::less<EHeroTrainableStatType>,std::allocator<std::pair<EHeroTrainableStatType_const_,SExpSpendUndo>_>_>
    unsigned char UndoableAbilityBuys[0x10];                 // +0x16c map<EHeroAbility,SExpSpendUndo,std::less<EHeroAbility>,std::allocator<std::pair<EHeroAbility_const_,SExpSpendUndo>_>_>
    unsigned char ExperiencePrimitiveHandles[0x10];          // +0x17c vector<CEnginePrimitiveHandle,std::allocator<CEnginePrimitiveHandle>_>
    unsigned char ExperienceStrengthPrimitiveHandles[0x10];  // +0x18c vector<CEnginePrimitiveHandle,std::allocator<CEnginePrimitiveHandle>_>
    unsigned char ExperienceSkillPrimitiveHandles[0x10];     // +0x19c vector<CEnginePrimitiveHandle,std::allocator<CEnginePrimitiveHandle>_>
    unsigned char ExperienceWeaponMagicPrimitiveHandles[0x10]; // +0x1ac vector<CEnginePrimitiveHandle,std::allocator<CEnginePrimitiveHandle>_>
    unsigned char ExperienceAbilityMagicPrimitiveHandles[0x10]; // +0x1bc vector<CEnginePrimitiveHandle,std::allocator<CEnginePrimitiveHandle>_>
    unsigned char ExperiencePureMagicPrimitiveHandles[0x10]; // +0x1cc vector<CEnginePrimitiveHandle,std::allocator<CEnginePrimitiveHandle>_>
    bool          OpenedExperienceSpending;                  // +0x1dc
    unsigned char _pad_0x1dd[0x1];                           // +0x1dd
    unsigned char _pad_0x1de[0x2];                           // +0x1de
    unsigned char Animators[0xc];                            // +0x1e0 list<CSliderAnimator,std::allocator<CSliderAnimator>_>
    unsigned char AnimIt[0x4];                               // +0x1ec _List_iterator<std::_List_val<CSliderAnimator,std::allocator<CSliderAnimator>_>_>
    long          OrigGeneral;                               // +0x1f0
    long          OrigStrength;                              // +0x1f4
    long          OrigSkill;                                 // +0x1f8
    long          OrigWill;                                  // +0x1fc
    long          statIndicatorPos;                          // +0x200
    long          statIndicatorMax;                          // +0x204
    CYesNo*       pYesNo;                                    // +0x208
    unsigned char ExperienceSpendAdd[0x8];                   // +0x20c ExperienceSpendEvent
    unsigned char ExperienceSpendRemove[0x8];                // +0x214 ExperienceSpendEvent
    CViewport*    pViewport;                                 // +0x21c
    CViewport*    pTextViewport;                             // +0x220
    unsigned char ExperienceSpendValuesPool[0x10];           // +0x224 vector<CTCInventoryExperience::ExperienceSpendInfo,std::allocator<CTCInventoryExperience::ExperienceSpendInfo>_>
    unsigned char ExperienceSpendValuesStats[0x10];          // +0x234 vector<CTCInventoryExperience::ExperienceSpendInfo,std::allocator<CTCInventoryExperience::ExperienceSpendInfo>_>
    unsigned char ExperienceSpendValuesAbility[0x10];        // +0x244 vector<CTCInventoryExperience::ExperienceSpendInfo,std::allocator<CTCInventoryExperience::ExperienceSpendInfo>_>
    long          ExperienceSpendMagnitude;                  // +0x254
    unsigned char MenuDisplayListExperienceItem[0x10];       // +0x258 vector<long,std::allocator<long>_>
    bool          CreateBecauseOfUndo;                       // +0x268
    unsigned char _pad_0x269[0x3];                           // +0x269
    long          NumItems;                                  // +0x26c
    bool          DestroyEverything;                         // +0x270
    unsigned char _pad_0x271[0x3];                           // +0x271
    unsigned char PreviousScreenType[0x4];                   // +0x274 EScreenType
    unsigned char UpgradableStats[0x10];                     // +0x278 map<EHeroTrainableStatType,NUISystem::CComponent_*,std::less<EHeroTrainableStatType>,std::allocator<std::pair<EHeroTrainableStatType_const_,NUISystem::CComponent_*>_>_>
    unsigned char UpgradableAbilities[0x10];                 // +0x288 map<EHeroAbility,NUISystem::CComponent_*,std::less<EHeroAbility>,std::allocator<std::pair<EHeroAbility_const_,NUISystem::CComponent_*>_>_>
    unsigned char ExpSliders[0x10];                          // +0x298 vector<NUISystem::CComponent_*,std::allocator<NUISystem::CComponent_*>_>
    unsigned char TutorialsPlayed[0x10];                     // +0x2a8 vector<CCharString,std::allocator<CCharString>_>
    float         AccumFadeTime;                             // +0x2b8
    CComponent*   PMousePointer;                             // +0x2bc
    float         Counter;                                   // +0x2c0
    long          PointsGeneral;                             // +0x2c4
    long          PointsStrength;                            // +0x2c8
    long          PointsSkill;                               // +0x2cc
    long          PointsWill;                                // +0x2d0
    unsigned char _pad_0x2d4[0x4];                           // +0x2d4

    long GetOriginalExperience(long experienceType);
};
#pragma pack(pop)

FABLE_STATIC_ASSERT(sizeof(CTCInventoryExperience) == 0x2d8);
FABLE_STATIC_ASSERT(offsetof(CTCInventoryExperience, __vftable) == 0x0);
FABLE_STATIC_ASSERT(offsetof(CTCInventoryExperience, TutorialSoundHandles) == 0x14c);
FABLE_STATIC_ASSERT(offsetof(CTCInventoryExperience, UndoableStatBuys) == 0x15c);
FABLE_STATIC_ASSERT(offsetof(CTCInventoryExperience, UndoableAbilityBuys) == 0x16c);
FABLE_STATIC_ASSERT(offsetof(CTCInventoryExperience, ExperiencePrimitiveHandles) == 0x17c);
FABLE_STATIC_ASSERT(offsetof(CTCInventoryExperience, ExperienceStrengthPrimitiveHandles) == 0x18c);
FABLE_STATIC_ASSERT(offsetof(CTCInventoryExperience, ExperienceSkillPrimitiveHandles) == 0x19c);
FABLE_STATIC_ASSERT(offsetof(CTCInventoryExperience, ExperienceWeaponMagicPrimitiveHandles) == 0x1ac);
FABLE_STATIC_ASSERT(offsetof(CTCInventoryExperience, ExperienceAbilityMagicPrimitiveHandles) == 0x1bc);
FABLE_STATIC_ASSERT(offsetof(CTCInventoryExperience, ExperiencePureMagicPrimitiveHandles) == 0x1cc);
FABLE_STATIC_ASSERT(offsetof(CTCInventoryExperience, OpenedExperienceSpending) == 0x1dc);
FABLE_STATIC_ASSERT(offsetof(CTCInventoryExperience, Animators) == 0x1e0);
FABLE_STATIC_ASSERT(offsetof(CTCInventoryExperience, AnimIt) == 0x1ec);
FABLE_STATIC_ASSERT(offsetof(CTCInventoryExperience, OrigGeneral) == 0x1f0);
FABLE_STATIC_ASSERT(offsetof(CTCInventoryExperience, OrigStrength) == 0x1f4);
FABLE_STATIC_ASSERT(offsetof(CTCInventoryExperience, OrigSkill) == 0x1f8);
FABLE_STATIC_ASSERT(offsetof(CTCInventoryExperience, OrigWill) == 0x1fc);
FABLE_STATIC_ASSERT(offsetof(CTCInventoryExperience, statIndicatorPos) == 0x200);
FABLE_STATIC_ASSERT(offsetof(CTCInventoryExperience, statIndicatorMax) == 0x204);
FABLE_STATIC_ASSERT(offsetof(CTCInventoryExperience, pYesNo) == 0x208);
FABLE_STATIC_ASSERT(offsetof(CTCInventoryExperience, ExperienceSpendAdd) == 0x20c);
FABLE_STATIC_ASSERT(offsetof(CTCInventoryExperience, ExperienceSpendRemove) == 0x214);
FABLE_STATIC_ASSERT(offsetof(CTCInventoryExperience, pViewport) == 0x21c);
FABLE_STATIC_ASSERT(offsetof(CTCInventoryExperience, pTextViewport) == 0x220);
FABLE_STATIC_ASSERT(offsetof(CTCInventoryExperience, ExperienceSpendValuesPool) == 0x224);
FABLE_STATIC_ASSERT(offsetof(CTCInventoryExperience, ExperienceSpendValuesStats) == 0x234);
FABLE_STATIC_ASSERT(offsetof(CTCInventoryExperience, ExperienceSpendValuesAbility) == 0x244);
FABLE_STATIC_ASSERT(offsetof(CTCInventoryExperience, ExperienceSpendMagnitude) == 0x254);
FABLE_STATIC_ASSERT(offsetof(CTCInventoryExperience, MenuDisplayListExperienceItem) == 0x258);
FABLE_STATIC_ASSERT(offsetof(CTCInventoryExperience, CreateBecauseOfUndo) == 0x268);
FABLE_STATIC_ASSERT(offsetof(CTCInventoryExperience, NumItems) == 0x26c);
FABLE_STATIC_ASSERT(offsetof(CTCInventoryExperience, DestroyEverything) == 0x270);
FABLE_STATIC_ASSERT(offsetof(CTCInventoryExperience, PreviousScreenType) == 0x274);
FABLE_STATIC_ASSERT(offsetof(CTCInventoryExperience, UpgradableStats) == 0x278);
FABLE_STATIC_ASSERT(offsetof(CTCInventoryExperience, UpgradableAbilities) == 0x288);
FABLE_STATIC_ASSERT(offsetof(CTCInventoryExperience, ExpSliders) == 0x298);
FABLE_STATIC_ASSERT(offsetof(CTCInventoryExperience, TutorialsPlayed) == 0x2a8);
FABLE_STATIC_ASSERT(offsetof(CTCInventoryExperience, AccumFadeTime) == 0x2b8);
FABLE_STATIC_ASSERT(offsetof(CTCInventoryExperience, PMousePointer) == 0x2bc);
FABLE_STATIC_ASSERT(offsetof(CTCInventoryExperience, Counter) == 0x2c0);
FABLE_STATIC_ASSERT(offsetof(CTCInventoryExperience, PointsGeneral) == 0x2c4);
FABLE_STATIC_ASSERT(offsetof(CTCInventoryExperience, PointsStrength) == 0x2c8);
FABLE_STATIC_ASSERT(offsetof(CTCInventoryExperience, PointsSkill) == 0x2cc);
FABLE_STATIC_ASSERT(offsetof(CTCInventoryExperience, PointsWill) == 0x2d0);

#endif // FABLE_ENGINE_CTCINVENTORYEXPERIENCE_H
