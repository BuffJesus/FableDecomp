#pragma once

#include "fable_string.h"
#include "rebuild_abi.h"
#include "fable_frontend_component_types.h"

class CGame;

class CGameComponent : public CBaseClassNonCopyable, public CDeviceResetCallback
{
public:
    explicit CGameComponent(CGame& game);
    virtual ~CGameComponent();
    virtual void Init();
    virtual bool Run(CGameComponent** nextComponent);
    virtual void ChangeTextureColourDepth(long depth);
    virtual void SetQuit();

    bool Quit;
    bool Running;
    CGame* Game;
};

class CMainGameComponentInit
{
public:
    // Ego_r PDB names; retail Play constructs these at +0/+4/+8/+0xC.
    CWideString InitialWorldName;
    CWideString InitialWorldHolySiteName;
    CCharString InitialQuestName;
    CWideString SaveGameName;
};

struct CFrontendGameComponentInit
{
    fable_u32 value;
};

typedef CFrontendGameComponentInit CNewFrontendGameComponentInit;

class CMainGameComponent : public CGameComponent
{
public:
    static void FABLE_FASTCALL GenerateMetFilesFromLugFiles();

    CMainGameComponent(
        CGame& game,
        const CMainGameComponentInit& init);

private:
    fable_u8 unknown10_[0x161D8];
};

class CFrontendGameComponent : public CGameComponent
{
public:
    CFrontendGameComponent(
        CGame& game,
        const CFrontendGameComponentInit& init);

private:
    fable_u8 unknown10_[0x1E50];
};

class CNewFrontendGameComponent : public CGameComponent
{
public:
    CNewFrontendGameComponent(
        CGame& game,
        const CNewFrontendGameComponentInit& init);

    static void CompileDefs();
    virtual ~CNewFrontendGameComponent();
    virtual void Init();
    virtual bool Run(CGameComponent** nextComponent);
    virtual void ChangeTextureColourDepth(long depth);
    virtual void SetQuit();
    virtual void OnPreDeviceReset();
    virtual bool OnPostDeviceReset();
    void InitialiseDefs();

    // PDB member names, retail offsets. Donor vectors were 16 bytes; retail
    // vectors are 12. The two reductions shift SaveGame and subsequent fields.
    FableFrontendCountedStorage<const CFontBank> m_pMenuFont; // +0x10
    int CurrentMovement;
    bool StartedRepeatingMovement;
    double TimeLastMovementHappened;                       // +0x20
    bool m_bAllowInputs;
    bool m_bCreateGame;
    bool CreateDevFrontEnd;
    CMainGameComponentInit m_bInitMainGame;                 // +0x2C
    const CFrontEndDef* m_pFrontEndDef;
    FableFrontendCountedStorage<CGraphicDataBank> m_pFrontEndGraphicBank;
    FableFrontendCountedStorage<CMeshDataBank> m_pMeshBank;
    FableFrontendCountedStorage<CASoundBank> PSampleBank;
    FableFrontendCountedStorage<CIEngine> m_pEngine;
    FableFrontendCountedStorage<NGameText::CDataBank> m_pTextBank;
    CInputManager* m_pInputManager;                        // +0x68 (not set by ctor)
    FableFrontendVectorStorage<CWideString> FileNames;     // +0x6C
    FableFrontendVectorStorage<CDateAndTime> FileTimes;    // +0x78
    CWideString SaveGame;                                 // +0x84
    CTexture* PAVITextureShow;
    CTexture* PAVITextureDecode;
    FableFrontendBoxI AVISrcBox;
    FableFrontendBoxI AVIDstBox;
    bool AVIPlaying;                                     // +0xB0
    bool XMVCodeLoaded;
    CFrontEndManager* m_pFrontEndManager;
    float m_fLastUpdateTime;
    float m_fLastRenderTime;
    double m_fLastRealUpdateTime;                         // +0xC0
    bool CreatedMarkerFile;
    FableFrontendInterpolationSet m_LastInterp;
    CStopWatch m_RepeatTimer;                             // +0xD8
    float m_fRepeatDelay;
    unsigned long m_dwDPadTriggerState;
    CStopWatch m_attractModeTimer;                        // +0x100
    CStopWatch m_inputDelayTimer;                         // +0x120
    long LastActiveJoystickDeviceNumber;
    bool WindowsMediaPlayerInstalled;                    // +0x144
};

class CGame
{
public:
    void Play();

private:
    fable_u8 unknown00_[8];
    CGameComponent* CurrentGameComponent;
    fable_u8 ParameterBuffer[0x200];
    bool Quit;
};

extern fable_u8 g_FableCompileFrontendDefinitions_013B8648;
extern fable_u8 g_FableStartMainGame_013B8605;
extern fable_u8 g_FableUseLegacyFrontend_013B8642;
extern CGameComponent* g_FableRetiredGameComponent_013B7D58;
extern CWideString g_FableMainGameStartupPath_013B7D5C;

FABLE_STATIC_ASSERT(sizeof(CMainGameComponentInit) == 0x10);
FABLE_STATIC_ASSERT(sizeof(CGameComponent) == 0x10);
FABLE_STATIC_ASSERT(sizeof(CFrontendGameComponentInit) == 0x04);
FABLE_STATIC_ASSERT(sizeof(CMainGameComponent) == 0x161E8);
FABLE_STATIC_ASSERT(sizeof(CFrontendGameComponent) == 0x1E60);
FABLE_STATIC_ASSERT(sizeof(CNewFrontendGameComponent) == 0x148);
FABLE_STATIC_ASSERT(sizeof(CGame) == 0x210);
