#pragma once

#include "fable_string.h"

void* FABLE_CDECL FableUserProfileManagerAllocate(fable_u32 size);

class CUserProfileManager
{
public:
    CUserProfileManager();

    __forceinline static void* operator new(size_t size)
    {
        return FableUserProfileManagerAllocate(size);
    }

    void SetProfileName(
        const CWideString& profileName,
        bool loadProfile);

    // FableWin ?Get@CUserProfileManager@@SIAAV1@XZ (0x0040D2A0) and
    // ?Apply@CUserProfileManager@@QAEXXZ (0x0040BC80); caller:
    // CMainGameComponent::Run 0x004189C2.
    static CUserProfileManager& FABLE_FASTCALL Get();
    void Apply();

    // FableWin ?GetAutoSaveCheckPointPathName@CUserProfileManager@@QBE?AVCWideString@@XZ
    // (retail 0x00408040; caller CWorld::DeleteCheckPointSave 0x0049E740).
    CWideString GetAutoSaveCheckPointPathName() const;
    // Ego_r ?GetDebugSaveDir@CUserProfileManager@@QBE?AVCWideString@@XZ, retail 0x00406780.
    CWideString GetDebugSaveDir() const;

private:
    void Load();
    void Save();

protected:
    // FableWin ?ApplyValues@CUserProfileManager@@IAEXXZ (retail 0x00407370)
    // and ?ApplyVideoValues@CUserProfileManager@@IAEXXZ (retail 0x0040A7F0):
    // the two callees of Apply (FableWin Apply 0x01880030 calls them in order).
    void ApplyValues();
    void ApplyVideoValues();

private:
    void* primaryVTable_;
    void* secondaryVTable_;
    CWideString profileName_;
    // Was one opaque0C_[0x134] block; split (layout unchanged) to name the
    // Ego_r member Apply 0x0040BC80 reads at +0x33.
    fable_u8 opaque0C_[0x33 - 0x0C];
    bool ApplyVideoValuesOnLoad;                 // +0x33
    fable_u8 opaque34_[0x140 - 0x34];
};

void FABLE_FASTCALL FableDestroyUserProfileManager_0040d830(void* object);

struct CCountedUserProfileManager
{
    CUserProfileManager* object;
    void* reference;

    void operator=(CUserProfileManager* value);
};

extern CCountedUserProfileManager
    g_CUserProfileManagerSingleton_013B7D4C;

FABLE_STATIC_ASSERT(sizeof(CUserProfileManager) == 0x140);
FABLE_STATIC_ASSERT(sizeof(CCountedUserProfileManager) == 0x08);
