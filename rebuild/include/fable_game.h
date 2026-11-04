#pragma once

#include "fable_base_class.h"
#include "fable_counted_pointer.h"
#include "fable_disk_file.h"
#include "fable_game_event.h"
#include "fable_display_viewport.h"
#include "fable_cgame.h"
#include "fable_init_base_class.h"
#include "fable_scoped_ptr.h"
#include "fable_stlport_vector.h"
#include "fable_string.h"
#include "rebuild_abi.h"

class CGame;
class CInputManager;

// A polymorphic interface; components register it with the display manager
// (CDisplayManager::AddDeviceResetCallback) to hear about device resets.
#include "fable_device_reset_callback.h"

// RTTI: CGameComponent : CBaseClassNonCopyable (: CBaseClass),
// CDeviceResetCallback. CGameComponent(CGame&) is 0x0051D8E0.
class CGameComponent : public CBaseClassNonCopyable, public CDeviceResetCallback
{
public:
    explicit CGameComponent(CGame& game);
    // Inline and empty in retail: CGameComponent's own deleting destructor
    // 0x00415940 and ~CNewFrontendGameComponent 0x0042F50B both inline the
    // two vptr stores and call ~CBaseClassNonCopyable 0x0099A300.
    virtual ~CGameComponent() {}
    virtual void Init();
    virtual bool Run(CGameComponent** nextComponent) = 0;
    virtual void ChangeTextureColourDepth(long depth) = 0;
    virtual void SetQuit();

protected:
    bool Quit;
    bool Running;
    CGame* Game;
};

class CMainGameComponentInit
{
public:
    CWideString InitialWorldName;
    CWideString InitialWorldHolySiteName;
    CCharString InitialQuestName;
    CWideString SaveGameName;
};

class CFrontendGameComponentInit
{
public:
    CFrontendGameComponentInit() : Nowt(0) {}
    long Nowt;
};

class CNewFrontendGameComponentInit
{
public:
    long Nothing;
};

// Sub-objects whose constructors the component constructors call out of
// line. Only their retail sizes are modelled.
struct CInterpolationInfoSet
{
    CInterpolationInfoSet() : GTPredictedTimeSinceLastRenderFrame(0.0f), WFInterpolate(0.0f) {}
    float GTPredictedTimeSinceLastRenderFrame;
    float WFInterpolate;
};

class CInterpolationInfo
{
public:
    CInterpolationInfo();
    CInterpolationInfoSet Current;
    CInterpolationInfoSet Last;
    CInterpolationInfoSet PausedCurrent;
    CInterpolationInfoSet PausedLast;
    CInterpolationInfoSet BulletTimeCurrent;
    CInterpolationInfoSet BulletTimeLast;
    long WFServerCurrentTime;
};
FABLE_STATIC_ASSERT(sizeof(CInterpolationInfo) == 0x34);

class CGameEvent;
class CGameEventPackage;
class CMainGameComponent;

class CGameEventPackageSet
{
public:
    CGameEventPackageSet();

    // FableWin names; retail addresses from CMainGameComponent's callers
    // (bodies match Ego_r's by call order).
    unsigned long GetNoPackages() const;                                  // 0x009F1750
    const CGameEventPackage* GetPackage(unsigned long index) const;       // 0x009F1730
    long CompressIntoBuffer(unsigned char* buffer) const;                 // 0x009F19A0
    void InitFromCompressedBuffer(const unsigned char* buffer);           // 0x009F1AC0
    // FableWin ?Clear@CGameEventPackageSet@@QAEXXZ (retail 0x009F1720) and
    // ?AddPackage@CGameEventPackageSet@@QAEXPBVCGameEventPackage@@@Z (0x009F16F0).
    void Clear();
    void AddPackage(const CGameEventPackage* package);

private:
    unsigned long NoPackages;
    CGameEventPackage Packages[50];
};

FABLE_STATIC_ASSERT(sizeof(CGameEventPackageSet) == 0x13A14);

// FableWin/Ego_r: CNetworkClient : CInitBaseClass (vptr + Valid in its first
// 8 bytes; retail CMainGameComponent::Shutdown 0x004175E5 calls
// CInitBaseClass::IsValid 0x0099A330 on &Client).
class CNetworkClient : public CInitBaseClass
{
public:
    CNetworkClient();
    // ?Uninitialise@CNetworkClient@@QAEXXZ (retail 0x004AE990).
    void Uninitialise();
    // FableWin ?WaitForSync@CNetworkClient@@QAEXXZ; empty in retail
    // (0x004AE9C0, just before Update 0x004AE9D0). Its only caller is
    // CMainGameComponent::Run 0x004189C2.
    void WaitForSync();
    // ?IsFreeToRender@CNetworkClient@@QBE_NXZ, retail 0x004AEA70.
    bool IsFreeToRender() const;
    // ?Update@CNetworkClient@@QAEXJKK@Z, retail 0x004AE9D0; CMainGameComponent::
    // Init 0x004184BD passes (CurrentServerFrame, PeekWorldChecksum(), WorldSeed).
    void Update(long serverFrame, unsigned long worldChecksum, unsigned long worldSeed);
    // ?GetGameEventPackageSet@CNetworkClient@@QAE_NPAVCGameEventPackageSet@@@Z,
    // retail 0x004AEBA0.
    bool GetGameEventPackageSet(CGameEventPackageSet* packageSet);
    // ?InitialiseAsLocal@CNetworkClient@@QAE_NPAVCMainGameComponent@@@Z (retail
    // 0x004AE940) and ?SendGameEvent@CNetworkClient@@QAEXPBVCGameEvent@@@Z
    // (retail 0x004AE9A0); callers CMainGameComponent::CreatePlayers 0x004166A8
    // and AddGameEvent 0x0041649C.
    bool InitialiseAsLocal(CMainGameComponent* component);
    void SendGameEvent(const CGameEvent* event);
    // ?IsLocal@CNetworkClient@@QBE_NXZ; inline in retail (CMainGameComponent::
    // Update 0x00418289 reads Client+0x2662, Ego_r's LocalGame).
    bool IsLocal() const
    {
        return LocalGame;
    }

private:
    // FableWin ?GetLocalGameEventPackageSet@CNetworkClient@@AAE_NPAVCGameEventPackageSet@@@Z
    // (retail 0x004AEAA0).
    bool GetLocalGameEventPackageSet(CGameEventPackageSet* packageSet);
    // FableWin ?CheckForLocalFrameUpdate@CNetworkClient@@ABE_NXZ; inline in retail
    // (0x004AEAA0 calls CMainGameComponent::IsTimeForServerUpdate(LocalFrame) directly).
    bool CheckForLocalFrameUpdate() const;

    // Ego_r.pdb names/offsets (retail agrees: Update 0x004AE9D0 and
    // GetLocalGameEventPackageSet 0x004AEAA0). The old unknown08_/unknown2663_
    // pads were split to name them; the layout is unchanged.
    fable_u8 unknown08_[0x2010 - 0x08];                  // Server, ReceiveBuffer
    CGameEventPackage LocalEventPackage;                 // +0x2010 (0x648)
    fable_u8 unknown2658_[0x2662 - 0x2658];              // LastUpdateTime, FirstTime, Host
    bool LocalGame;                                      // +0x2662
    fable_u8 unknown2663_[0x266C - 0x2663];              // pad, LocalPlayer, HostPlayer
    long LocalFrame;                                     // +0x266C
    unsigned long Checksum1;                             // +0x2670
    unsigned long Checksum2;                             // +0x2674
    CMainGameComponent* GameComponent;                   // +0x2678
    fable_u8 unknown267C_[0x2680 - 0x267C];
};

struct CEventPackageFileHeader
{
    long NoPlayers;
};

// Retail is 8 bytes under Ego_r's 0x38: its CArray<float> is a 12-byte
// STLport vector and the trailing double packs down.
// Retail vtable 0x0122F168: scalar deleting destructor 0x0041977D, then the
// inherited CInitBaseClass::Validate 0x0099A340.
class CFrameRateSmoother : public CInitBaseClass
{
public:
    CFrameRateSmoother();
    bool Init(long maxNoTimes, long noFramesToChangeFrameRateOver);
    void Uninitialise();
    void AddFrameTime(double time);
    double GetSmoothedTime() const;

private:
    void CalculateSmoothedTime();
    _STL::vector<float> Times;
    long MaxNoTimes;
    long NoTimes;
    long NoFramesToChangeFrameRateOver;
    long FirstTime;
    long LastTime;
    double SmoothedTime;
};

class CRoughFrameCounter
{
public:
    CRoughFrameCounter();
    void NewFrame();

    unsigned long FrameStart;
    float FPS;
};

class CEngineInterpolationInfoSet
{
public:
    CEngineInterpolationInfoSet()
        : GTPredictedRenderAt(0.0f),
          GTPredictedTimeSinceLastRenderFrame(0.0f),
          WFInterpolate(0.0f)
    {
    }

    float GTPredictedRenderAt;
    float GTPredictedTimeSinceLastRenderFrame;
    float WFInterpolate;
};

#include "fable_stopwatch.h"

// Ego_r.pdb layout (0x70). Its constructor stays out of line in the /Os
// frontend TU (0x0042FA30, zero-fills every field).
class CEngineInterpolationInfo
{
public:
    CEngineInterpolationInfo();

    CEngineInterpolationInfoSet Current;
    CEngineInterpolationInfoSet Last;
    CEngineInterpolationInfoSet PausedCurrent;
    CEngineInterpolationInfoSet PausedLast;
    CEngineInterpolationInfoSet BulletTimeCurrent;
    CEngineInterpolationInfoSet BulletTimeLast;
    long WFServerCurrentTime;
    CEngineInterpolationInfoSet InterpolationSetList[3];
};

// Ego_r.pdb layout (0x10); retail vtable 0x01230C34, ctor 0x0042DB40.
class CXMVPlayerBuffers : public CBaseClassNonCopyable
{
public:
    CXMVPlayerBuffers();
    virtual ~CXMVPlayerBuffers();

private:
    char* InputBuffer;
    char* PlaybackBuffer;
    unsigned long CurrentSize;
};

#include "fable_def_pointer.h"

class CASoundBank;
class CDateAndTime;

// CFrontEndDef (retail 0x54 bytes, vAttractModeMovie at +0x3C) now has its
// own header with the full def-class hierarchy.
#include "fable_frontend_def.h"

#include "fable_date_and_time.h"
class CDisplayEngine;
class CGameEvent;
class CGameEventPackage;
class CGameDefinitionManager;
class CFontBank;
class CFrontEndManager;
class CFrontEndDef;
class CGamePlayerInterface;
class CGraphicDataBank;
class CIEngine;
class CLUA;
class CMeshDataBank;
class CPlayerManager;
class CTexture;
class CWorld;
namespace NGameText { class CDataBank; }
namespace NUISystem { class CFrontEndManager; }

enum ECursorMovement
{
    CURSOR_MOVEMENT_NONE
};

double FABLE_FASTCALL GFGetTime();

extern CDisplayManager* g_FableDisplayManager_013B8390;

// ??$GFLimitReturn@N@@YIABNABN00@Z: out of line in retail (0x0041919C,
// CMainGameComponent::Render calls it); declared only so callers keep the call.
template<class T>
const T& FABLE_FASTCALL GFLimitReturn(const T& value, const T& minimum, const T& maximum);

// Ego_r.pdb ?GForceRenderFramesPerSec@@3JA and ?GRenderFrame@@3KA.
extern long g_ForceRenderFramesPerSec_013B8630;
extern unsigned long g_RenderFrame_013B7D6C;
// Ego_r.pdb ?GWorldFrame@@3KA and ?GDeleteCheckPointSaveOnInitWorld@@3_NA
// (CMainGameComponent::Init 0x004184BD zeroes the first and consumes the second).
extern unsigned long g_WorldFrame_013B7D70;
extern bool g_DeleteCheckPointSaveOnInitWorld_013B7C90;

// MainGameComponent.cpp globals (Ego_r.pdb names, retail addresses from the
// CMainGameComponent code that reads them).
class CVersion;
extern CVersion g_Version_013B85E4;                        // ?GVersion@@3VCVersion@@A
extern bool g_SaveEventPackages_013B85F7;                   // ?GSaveEventPackages@@3_NA
extern bool g_LoadEventPackages_013B85F8;                   // ?GLoadEventPackages@@3_NA
extern CWideString g_EventPackageFileName_013B8660;         // ?GEventPackageFileName@@3VCWideString@@A
extern bool g_RenderOnlyWhenRecordingMovie_013B860C;        // ?GRenderOnlyWhenRecordingMovie@@3_NA
extern bool g_RecordMovie_013B8629;                         // ?GRecordMovie@@3_NA

// Retail vtable 0x0122F180 has ten primary slots: the five CGameComponent
// slots, then Update 0x418289, GetInputs 0x416E78, Render 0x417001, PostInit
// 0x416953 and IsEditorActive 0x4197B0 (checked by gate_game_component_abi.py).
class CGameEventDispatch;
class CMainGameComponent : public CGameComponent
{
public:
    virtual void Init();
    virtual bool Run(CGameComponent** nextComponent);
    virtual void ChangeTextureColourDepth(long depth);
    virtual void SetQuit();
    virtual void OnPreDeviceReset();
    virtual bool OnPostDeviceReset();

protected:
    virtual void Update();
    virtual void GetInputs();
    virtual void Render();
    virtual void PostInit();

public:
    virtual bool IsEditorActive() const;

    // --- CMainGameComponent::Shutdown / GetInputs / Init*Engine lane ---
    // FableWin.pdb names; retail addresses in the trailing comments.
    void SetDisplayMode(unsigned long width, unsigned long height, unsigned char depth); // retail 0041641F
    void SetExclusiveMode(bool exclusive);                              // 0x004163B7
    // ?PreChangeResolution@@QAEXXZ: inline in retail (SetExclusiveMode calls
    // CDisplayEngine::PreChangeResolution 0x00434A10 directly); defined where
    // CDisplayEngine is complete.
    void PreChangeResolution();
    void PostChangeResolution(unsigned long width, unsigned long height, unsigned long depth); // 0x00416365
    unsigned long PeekWorldChecksum() const;                            // 0x00416392
    unsigned long* GetLocalSeed() const { return const_cast<unsigned long*>(&LocalSeed); }
    unsigned long PeekWorldSeed() const { return WorldSeed; }           // inline (+0x1613C)
    long GetCurrentServerFrame() const { return CurrentServerFrame; }   // inline (+0x48)
    const CFontBank& PeekDebugFont() const { return *PDebugFont.Get(); } // inline (+0x16144)
    // ?PeekDefinitionManager@@QBEABVCGameDefinitionManager@@XZ: inline in retail
    // (a call to CGameDefinitionManager::Get 0x0044C6B0); defined where
    // CGameDefinitionManager is complete.
    const CGameDefinitionManager& PeekDefinitionManager() const;
    void AddGameEvent(const CGameEvent& event);                         // 0x0041649C

protected:
    bool EventIsSystemEvent(const CGameEvent* event);                   // 0x00415FF2
    void ProcessEvent(const CGameEvent* event);                         // 0x00415FE0
    void CheckSync(const CGameEventPackage* package);                   // 0x004165E8
    void UninitEventPackageLoading();                                   // 0x004160A6
    void UninitEventPackageSaving();                                    // 0x00416122
    void InitDisplayEngine();                                           // 0x00417418
    void InitParticleEngine();                                          // 0x004174F1

    // Ego_r.pdb ?PMainGameComponent@CMainGameComponent@@1PAV1@A (retail
    // 0x013B86A0; fable_boot.h's g_FableProgressTextBankSlot_013B86A0 is the
    // same address). Shutdown 0x004175E5 clears it.
    static CMainGameComponent* PMainGameComponent;
public:
    // FableWin ?Get@CMainGameComponent@@SIPAV1@XZ (public); inline in retail
    // (a load of PMainGameComponent, e.g. CWorld::ActivateWorld 0x0049F180).
    CPlayerManager& GetPlayerManager() { return *PPlayerManager; }
    static CMainGameComponent* FABLE_FASTCALL Get()
    {
        return PMainGameComponent;
    }
protected:
    // --- end lane ---

    // --- CMainGameComponent::Init 0x004184BD lane: its remaining callees
    // (FableWin names and access, all protected). ---
    bool InitDefinitions(bool compile);                  // 0x00416005 ?InitDefinitions@@IAE_N_N@Z
    void InitPlayerManager();                            // 0x0041732A
    void InitWorld();                                    // 0x0041735A
    void CreatePlayers();                                // 0x004166A8
    void InitSound();                                    // 0x00417A58 ?InitSound@@IAEXXZ
    // ?AppReinitFunc@CMainGameComponent@@KIXPAX@Z (protected static fastcall),
    // retail 0x004167DA; Init registers it with the system manager.
    static void FABLE_FASTCALL AppReinitFunc(void* context);
    // --- end Init lane ---

public:

    // FableWin ?UpdateRegular / ?UpdateForcedTickSpeed @@QAEXXZ: the two
    // per-frame update paths Run 0x004189C2 picks between (on ForceUpdateTick).
    void UpdateRegular();                                // 0x004162B5
    void UpdateForcedTickSpeed();                        // 0x00417747

protected:
    // FableWin ?Shutdown@CMainGameComponent@@IAEXXZ.
    void Shutdown();                                     // 0x004175E5
    // FableWin ?BeginInputLoading / ?BeginInputSaving@CMainGameComponent@@IAEXABVCWideString@@@Z.
    // Retail 0x00416268 calls CInputManager::StreamInputsToFile (Saving) and
    // 0x0041627F StreamInputsFromFile (Loading); Ego_r has the same order
    // (BeginInputSaving 0x004154CA, BeginInputLoading 0x004154E1).
    void BeginInputLoading(const CWideString& fileName); // 0x0041627F
    void BeginInputSaving(const CWideString& fileName);  // 0x00416268
    // FableWin ?InitEventPackageLoading / ?InitEventPackageSaving@CMainGameComponent@@
    // IAE_NPB_W@Z (retail 0x00416056 / 0x004160CC; caller InitWorld 0x0041735A).
    bool InitEventPackageLoading(const wchar_t* fileName);
    bool InitEventPackageSaving(const wchar_t* fileName);
    // FableWin ?ProcessEventPackage@CMainGameComponent@@IAEXPBVCGameEventPackage@@@Z
    // (retail 0x00416670; caller UpdateFromEventPackageSet 0x0041726D).
    void ProcessEventPackage(const CGameEventPackage* package);
    // FableWin ?InitialiseText / ?InitialiseFonts @@IAEXXZ, ?InitGraphics@@IAE_NXZ
    // and ?InitLUA@CMainGameComponent@@IAEXABVCCharString@@@Z (all protected;
    // retail's InitLUA 0x004BBC00 is an empty `ret 4`).
    void InitialiseText();                               // 0x00416832
    void InitialiseFonts();                              // 0x004168DC
    bool InitGraphics();                                 // 0x00416C8A
    void InitLUA(const CCharString& scriptName);         // 0x004BBC00
    void UpdateAverageFrameDuration(double duration);
    float GetAverageFrameDuration() const;
    double GetCurrentGameTime() const;
    double GetPredictedTimeSinceLastRenderFrame() const;
    void ForceRender(long framesPerSecond);                 // 0x004164E0
    double GetGameTimeOfNextPresentCompletion() const;      // 0x004166E2
    // ?ConvertGTToWF@CMainGameComponent@@IBENN@Z: inline in retail (Render
    // 0x00417001); Ego_r's out-of-line copy 0x004154A3 is gameTime / (1.0f / ConstantFPS).
    double ConvertGTToWF(double gameTime) const
    {
        return gameTime / (1.0f / GetConstantFPS());
    }

public:
    const CGameEventDispatch* PeekEventDispatchTableEntry(long type) const;
    static void FABLE_FASTCALL GenerateMetFilesFromLugFiles();
    // Ego_r.pdb ?AlwaysUpdateGameFrame@CMainGameComponent@@2_NA (retail 0x013B8688).
    static bool AlwaysUpdateGameFrame;
    // ?GetConstantFPS@CMainGameComponent@@SIJXZ; inline in retail.
    static long FABLE_FASTCALL GetConstantFPS()
    {
        return ConstantFPS;
    }

private:
    // Ego_r.pdb ?ConstantFPS@CMainGameComponent@@1JA (retail 0x01375550, 15) and
    // ?UseOldInterpolationTiming@CMainGameComponent@@1_NA (retail 0x013B86A4).
protected:
    static const CGameEventDispatch EventDispatch[87];
private:
    friend struct CGameEventDispatchTestAccess;
    static long ConstantFPS;
    static bool UseOldInterpolationTiming;

public:

    // FableWin ?ProfileUpdateTime / ?ProfileRenderTime@CMainGameComponent@@2NA:
    // UpdateRegular 0x004162B5 times Update() and Render() into these.
    static double ProfileUpdateTime;                     // 0x013B8690
    static double ProfileRenderTime;                     // 0x013B8698

    // Callees of Update 0x00418289 / UpdateForcedTickSpeed 0x00417747
    // (FableWin names and access). AddGameEvent / GetCurrentServerFrame are
    // declared in the Shutdown/GetInputs lane block above.
    bool IsControllerDisconnected() const;               // 0x00416296
    bool IsTimeForServerUpdate(long frame) const;        // 0x0041674A
    void SignalFrameUpdate();                            // 0x00416047

protected:
    bool GetEventPackageSetFromSave(CGameEventPackageSet* packageSet);        // 0x00416148
    void UpdateFromEventPackageSet(const CGameEventPackageSet* packageSet);   // 0x0041726D
    void AddEventPackageSetToSave(const CGameEventPackageSet* packageSet);    // 0x004161A7
    long GetFrameDifferenceFromCurrent(long frame) const;                     // 0x004167A7

public:

    CMainGameComponent(
        CGame& game,
        const CMainGameComponentInit& init);
    virtual ~CMainGameComponent();

private:
    CASoundBank* PSampleBank;
    CCountedPointer<NGameText::CDataBank> PTextBank;
    boost::scoped_ptr<CPlayerManager> PPlayerManager;
    boost::scoped_ptr<CGamePlayerInterface> PPlayerInterface;
    boost::scoped_ptr<CWorld> PWorld;
    boost::scoped_ptr<CDisplayEngine> PDisplayEngine;
    CCountedPointer<CLUA> PLUA;
    bool ForceUpdateTick;
    float ForceUpdateTickSpeedMultipler;
    float ForceUpdateTickSpeedDesiredFramerate;
    long ForceUpdateNoFailedUpdates;
    bool FirstWorldFrameUpdate;
    long CurrentServerFrame;
    long InputServerFrame;
    long LastGameTurnForceRendered;
    double CurrentFrameStartGameTime;
    double GameStartTime;
    double LastFrameRenderDuration;
    CInterpolationInfo LastInterpolationInfo;
    CGameEventPackageSet EventPackageSet;
    CNetworkClient Client;
    unsigned long NoRenderFramesSinceLastGameUpdate;
    unsigned long WorldSeed;
    unsigned long LocalSeed;
    CCountedPointer<CFontBank> PDebugFont;
    CCountedPointer<CFontBank> PCutSceneMainFont;
    CDiskFileWin32 EventPackageFile;
    bool LoadingEventPackages;
    bool SavingEventPackages;
    CEventPackageFileHeader EventPackageFileHeader;
    CFrameRateSmoother FrameRateSmoother;
    double LastRenderFrameStartTime;
    float TimePassedSinceLastUpdate;
    float LastUpdateTime;
    bool WorldUpdateTurn;
    CRoughFrameCounter RoughFPSCounter;
    CGameComponent* NextComponentToRun;
    CCountedPointer<CGraphicDataBank> PMainGraphicBank;
    CMainGameComponentInit InitStructure;
    bool Initialised;
    bool AllowRender;
    bool Rendered;
    long DebugNoFramesUnableToRender;
};

class CFrontendGameComponent : public CGameComponent
{
public:
    virtual void Init();
    virtual bool Run(CGameComponent** nextComponent);
    virtual void ChangeTextureColourDepth(long depth);
    virtual void SetQuit();
    virtual void OnPreDeviceReset();
    virtual bool OnPostDeviceReset();

    CFrontendGameComponent(
        CGame& game,
        const CFrontendGameComponentInit& init);

private:
    fable_u8 unknown10_[0x1E50];
};

// Retail is 0x148: Ego_r's layout with FileNames/FileTimes as 12-byte
// STLport vectors and one extra bool at +0x144 (set true by the ctor).
class CNewFrontendGameComponent : public CGameComponent
{
public:
    virtual void Init();
    virtual bool Run(CGameComponent** nextComponent);
    virtual void ChangeTextureColourDepth(long depth);
    virtual void SetQuit();
    virtual void OnPreDeviceReset();
    virtual bool OnPostDeviceReset();

    CNewFrontendGameComponent(
        CGame& game,
        const CNewFrontendGameComponentInit& init);
    virtual ~CNewFrontendGameComponent();

    static void FABLE_FASTCALL CompileDefs();
    static void FABLE_FASTCALL CompileDefsPlatformSpecific(bool xbox);
    void InitialiseInput();
    void InitialiseDefs();
    bool InitialiseGraphics();
    void Update();
    bool IsControllerDisconnected();
    void Shutdown();                                     // 0x0042EBB6
    void Input();                                        // 0x0042E3EE
    void CleanUpInterfaces();                            // 0x0042DD28
    void ReleaseXMVCode();                               // 0x0042DBD8
    void Init2();                                        // 0x0042E98F
    void InitialiseEngine();                             // 0x0042E204
    void UpdateInterpolation(CEngineInterpolationInfo& info);   // 0x0042DBFA
    void Draw(const CEngineInterpolationInfo& info);            // 0x0042DF9E
    // FableWin ?InitialiseSounds@CNewFrontendGameComponent@@AAEXXZ (retail
    // 0x0042E17C; caller Init2 0x0042E98F).
    void InitialiseSounds();

private:
    CCountedPointer<const CFontBank> m_pMenuFont;
    ECursorMovement CurrentMovement;
    bool StartedRepeatingMovement;
    double TimeLastMovementHappened;
    bool m_bAllowInputs;
    bool m_bCreateGame;
    bool CreateDevFrontEnd;
    CMainGameComponentInit m_bInitMainGame;
    CDefPointer<const CFrontEndDef> m_pFrontEndDef;
    CCountedPointer<CGraphicDataBank> m_pFrontEndGraphicBank;
    CCountedPointer<CMeshDataBank> m_pMeshBank;
    CCountedPointer<CASoundBank> PSampleBank;
    CCountedPointer<CIEngine> m_pEngine;
    CCountedPointer<NGameText::CDataBank> m_pTextBank;
    CInputManager* m_pInputManager;
    _STL::vector<CWideString> FileNames;
    _STL::vector<CDateAndTime> FileTimes;
    CWideString SaveGame;
    CTexture* PAVITextureShow;
    CTexture* PAVITextureDecode;
    C2DBoxI AVISrcBox;
    C2DBoxI AVIDstBox;
    bool AVIPlaying;
    bool XMVCodeLoaded;
    // FableWin.pdb: NUISystem::CFrontEndManager* (retail +0xB4).
    NUISystem::CFrontEndManager* m_pFrontEndManager;
    float m_fLastUpdateTime;
    float m_fLastRenderTime;
    double m_fLastRealUpdateTime;
    bool CreatedMarkerFile;
    CEngineInterpolationInfoSet m_LastInterp;
    CStopWatch m_RepeatTimer;
    float m_fRepeatDelay;
    fable_u32 m_dwDPadTriggerState;
    CStopWatch m_attractModeTimer;
    CStopWatch m_inputDelayTimer;
    long LastActiveJoystickDeviceNumber;
    bool unknown144_;
};

extern fable_u8 g_FableCompileFrontendDefinitions_013B8648;
extern fable_u8 g_FableStartMainGame_013B8605;
extern fable_u8 g_FableUseLegacyFrontend_013B8642;
extern CGameComponent* g_FableRetiredGameComponent_013B7D58;
extern CWideString g_FableMainGameStartupPath_013B7D5C;

FABLE_STATIC_ASSERT(sizeof(CGameComponent) == 0x10);
FABLE_STATIC_ASSERT(sizeof(CMainGameComponentInit) == 0x10);
FABLE_STATIC_ASSERT(sizeof(CFrontendGameComponentInit) == 0x04);
FABLE_STATIC_ASSERT(sizeof(CNewFrontendGameComponentInit) == 0x04);
FABLE_STATIC_ASSERT(sizeof(CFrameRateSmoother) == 0x30);
FABLE_STATIC_ASSERT(sizeof(CMainGameComponent) == 0x161E8);
FABLE_STATIC_ASSERT(sizeof(CFrontendGameComponent) == 0x1E60);
FABLE_STATIC_ASSERT(sizeof(CNewFrontendGameComponent) == 0x148);
FABLE_STATIC_ASSERT(sizeof(CStopWatch) == 0x20);
