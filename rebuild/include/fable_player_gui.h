#pragma once

// CPlayerGui as retail reaches it: the CTBaseSingletonNoDefaultAlloc<CPlayerGui>
// singleton (?PClass@?$CTBaseSingletonNoDefaultAlloc@VCPlayerGui@@@@0V?$CCountedPointer@VCPlayerGui@@@@A,
// retail 0x013B8790) and its inline GetSubtitleRenderer (Ego_r SubtitleRenderer
// at +0x18, 0x128 bytes). Only what retail callers touch is modelled; do not
// include together with the generated flat "engine/CPlayerGui.h".

#include "fable_system_manager.h"
#include "fable_def_pointer.h"
class CPlayerGuiDef;
#include "fable_scoped_ptr.h"
#include "fable_hero_information_screens.h"

class CSubtitleRenderer
{
public:
    // FableWin ?IsInInteractiveState@CSubtitleRenderer@@QBE_NXZ, retail 0x00490A22.
    bool IsInInteractiveState() const;
    bool ClickPast(); // FableWin member; retail 00493281

private:
    fable_u8 unknown00_[0x128];
};

class CPlayerGui : public CTBaseSingletonNoDefaultAlloc<CPlayerGui>
{
public:
    // FableWin ?GetSubtitleRenderer@CPlayerGui@@QAEAAVCSubtitleRenderer@@XZ; inline.
    CSubtitleRenderer& GetSubtitleRenderer()
    {
        return SubtitleRenderer;
    }

    // FableWin ?InitialActivate@CPlayerGui@@QAEXXZ (retail 0x0043A380; caller
    // CWorld::ActivateWorld 0x0049F180).
    void InitialActivate();
    static const CPlayerGuiDef& FABLE_FASTCALL PeekGuiDef(); // retail 00438570
    NHeroInformationScreens::CManager* GetPHeroInfoScreensManager() const; // retail 00437CD0

private:
    static CDefPointer<const CPlayerGuiDef> PGuiDef; // retail 013B878C
    fable_u8 unknown04_[0x18 - 0x04];
    friend struct CPlayerGuiHeroInfoTestAccess;
    CSubtitleRenderer SubtitleRenderer;     // +0x18
    fable_u8 unknown140_[0x32C-0x140];
    boost::scoped_ptr<NHeroInformationScreens::CManager> PHeroInfoScreensManager; // +32C
};
