#pragma once

// The subsystem managers CSystemManager::Initialise (0x9A6610) builds.
// Hierarchies are retail RTTI; constructor signatures are FableWin.pdb's
// (CSoundManagerLH is retail-only).

#include "fable_base_class.h"
#include "fable_display_viewport.h"
#include "fable_gfuninitialise.h"
#include "fable_sound_manager.h"
#include "fable_stlport_list.h"
#include "fable_stlport_tree.h"
#include "fable_graphic_data_bank_init.h"

class CMeshDataBank;

// FableWin: ?GFInitMaths@@YIXXZ (0xA0DB60, reciprocal-root and trig tables) and
// ?GFInitVectorMath@NVectorMath@@YAXXZ (0xA5B850, SSE detection).
void FABLE_FASTCALL GFInitMaths();

namespace NVectorMath
{
    void GFInitVectorMath();
}

// ?GScratchBuffer@@3VCScratchBuffer@@A (0x13CB498).
class CScratchBuffer : public CInitBaseClass
{
public:
    bool Initialise(long size);
    void Uninitialise();
    // FableWin ?Lock@CScratchBuffer@@QAE_NPAPAX@Z / ?Unlock@@QAEXXZ (retail
    // 0x009F4980 / 0x009F49A0; callers CMainGameComponent::
    // GetEventPackageSetFromSave 0x00416148 / AddEventPackageSetToSave 0x004161A7).
    bool Lock(void** buffer);
    void Unlock();
    // FableWin ?GetByteSize@CScratchBuffer@@QBEJXZ / ?IsLocked@CScratchBuffer@@QBE_NXZ; inline in retail.
    long GetByteSize() const { return NoBytes; }
    bool IsLocked() const { return Locked; }
private:
    friend struct CScratchBufferTestAccess;
    fable_u8* Buffer;
    bool Locked;
    long NoBytes;
};
FABLE_STATIC_ASSERT(sizeof(CScratchBuffer)==0x14);

extern CScratchBuffer GScratchBuffer;

// Vtable 0x122DE50: OnPreDeviceReset, OnPostDeviceReset,
// ChangeTextureColourDepth(long).
#include "fable_device_reset_callback.h"

// RTTI: CInputManagerDX : CInputManager : CBaseClass. Ctor 0xA60050.
#include "fable_input_manager.h"

// RTTI: CSoundManagerLH : CASoundManager. Ctor 0xA3EF00 (0x80 bytes).
class CSoundManagerLH : public CASoundManager
{
public:
    CSoundManagerLH(const CSoundManagerInit& init, bool exclusiveMode);

private:
    fable_u32 unknown04_[(0x80 - 4) / 4];
};

#include "engine/CFontBank.h"

class CGraphicDataBank;
class CGraphicDataBankInit;

class CFontDataBank;
class CStreamingFontDataBank;
class CSymbolMap;
class CVertexBufferWin32;
// RTTI: CFontManager : CBaseClassNonCopyable. Ctor 0x9E2E30 (0x3C bytes).
class CFontManager : public CBaseClassNonCopyable
{
public:
    CFontManager(
        const CCharString& fontBankHandle,
        const CCharString& streamingFontBankHandle);
    virtual ~CFontManager();

    // Retail 0x009E2C80 (FableWin ?GetFont@CFontManager@@QAE?AV?$CCountedPointer@VCFontBank@@@@ABVCCharString@@@Z).
    CCountedPointer<CFontBank> GetFont(const CCharString& name);
    CCountedPointer<CFontBank> GetStaticFont(unsigned long index);
    CCountedPointer<CFontBank> GetStreamingFont(unsigned long index);
    void ReleaseFont(const CCharString& name);
    CVertexBufferWin32& GetFreeVertexBuffer();

    // FableWin ?SetLoadFontGlyphsAsynchronously@CFontManager@@QAEX_N@Z; inline
    // in retail (CNewFrontendGameComponent::Shutdown 0x0042EBB6 stores +0x38).
    void SetLoadFontGlyphsAsynchronously(bool asynchronously)
    {
        LoadFontGlyphsAsynchronously = asynchronously;
    }
    // Ego_d ?GetLoadFontGlyphsAsynchronously@CFontManager@@QBE_NXZ; inline in retail
    // (CStreamingFontDataBank::StreamGlyph 0x00A5F010 reads +0x38).
    bool GetLoadFontGlyphsAsynchronously() const
    {
        return LoadFontGlyphsAsynchronously;
    }

private:
    // Ego_r's CArray VertexBuffers is a 12-byte STLport vector in retail, so
    // LoadFontGlyphsAsynchronously sits at +0x38 (Ego_r +0x3C).
    friend struct CFontManagerTestAccess;
    // Authored access (not recovered friend metadata): CStreamingFontBank::Preload 0x009A2000 / GFUpdateFontTextureCache
    // adjust NumberAsyncGlyphsPending directly (no accessor in the debug PDBs).
    friend class CStreamingFontBank;
    CCountedPointer<CFontDataBank> PFontDataBank;
    CCountedPointer<CStreamingFontDataBank> PStreamingFontDataBank;
    CCountedPointer<CSymbolMap> PSymbols;
    CCountedPointer<CSymbolMap> PStreamingSymbols;
    _STL::vector<CVertexBufferWin32*> VertexBuffers;
    long CurrentVB;
    unsigned long NumberAsyncGlyphsPending;
    bool LoadFontGlyphsAsynchronously;  // +0x38
    fable_u8 unknown39_[3];
};

// RTTI: CGraphicBankManager : CBaseClassNonCopyable. Ctor 0x9F8690 (8 bytes).
class CGraphicBankManager : public CBaseClassNonCopyable
{
public:
    CGraphicBankManager();
    virtual ~CGraphicBankManager();
    class CGraphicDataBankInfo { public: CGraphicDataBankInit Init; CCharString BankHandle; CCountedPointer<CGraphicDataBank> GraphicDataBank; };

    // Retail 0x009F83D0 (FableWin ?OpenGraphicBank@CGraphicBankManager@@QAE?AV?$CCountedPointer@VCGraphicDataBank@@@@ABVCCharString@@ABVCGraphicDataBankInit@@_N2@Z).
    CCountedPointer<CGraphicDataBank> OpenGraphicBank(
        const CCharString& bankHandle,
        const CGraphicDataBankInit& init,
        bool asSurfaces,
        bool readSymbols);

    // FableWin ?CloseGraphicBank@CGraphicBankManager@@QAEXV?$CCountedPointer@VCGraphicDataBank@@@@@Z
    // (0x009F8290).
    void CloseGraphicBank(CCountedPointer<CGraphicDataBank> bank);

private:
    friend struct CBankManagerTestAccess;
    _STL::list<CGraphicDataBankInfo> GraphicDataBanks;
};

// RTTI: CMeshBankManager : CBaseClassNonCopyable, CDeviceResetCallback (+4).
// Ctor 0xA0A110 (0xC bytes).
class CMeshBankManager
    : public CBaseClassNonCopyable,
      public CDeviceResetCallback
{
public:
    CMeshBankManager();
    virtual ~CMeshBankManager();
    class CMeshDataBankInfo { public: CCharString BankHandle; CCountedPointer<CMeshDataBank> MeshDataBank; };
    virtual void OnPreDeviceReset();
    virtual bool OnPostDeviceReset();

    // FableWin ?CloseMeshBank@CMeshBankManager@@QAEXV?$CCountedPointer@VCMeshDataBank@@@@@Z
    // (0x00A09DC0).
    void CloseMeshBank(CCountedPointer<CMeshDataBank> bank);

private:
    friend struct CBankManagerTestAccess;
    _STL::list<CMeshDataBankInfo> MeshDataBanks;
};

// Performance profiler (not player save profiles). Ego_r member names;
// retail STLport containers put CProfileManager's tail at 0x70, sizeof 0x78.
class CProfileManager;
class CProfile
{
public:
    CProfile() : Manager(0) {}
    CProfile(const CProfile& other);                         // 0x009F9450
    CProfileManager* Manager;
    double StartTime, TotalTime, LastTime;
    bool Running;
    double Percentage;
    CWideString Name;
    long TreeDepth;
    bool NewNode;
    long Visible, CallCounter, UpdatedCallCounter;
};
FABLE_STATIC_ASSERT(sizeof(CProfile)==0x48);
class CProfileManager
{
public:
    struct CProfileTreeEntry {
        CProfile Profile;
        long ID;
        _STL::vector<CProfileTreeEntry*> Children;
        CProfileTreeEntry* Parent;
    };
    CProfileManager();                                      // 0x009F9160
    ~CProfileManager();
    void Update();                                         // 0x009F8C10
    void DrawProfiles(const C2DBoxF& box, const CFontBank& font); // 0x009F8D60
private:
    friend struct CProfileManagerTestAccess;
    bool UpPressed, DownPressed, TogglePressed, FlipAllPressed;
    _STL::_List_iterator<CProfile*> CursorPosition;
    bool AutoDisplayHierarchy, Valid;
    double ApplicationTimerStart, ApplicationTotalTime;
    bool MinUpdated;
    double MinApplicationTotalTime, StoredTotalTime;
    long NoTurns;
    CRenderManager2D* RenderManager;
    long DrawMode, EvalMode; // Enum names known; enumerator names not yet recovered.
    bool NewProfilesRegistered;
    _STL::list<CProfile*> ProfileList;
    _STL::map<long,CCharString> IDToProfileName;
    _STL::map<CCharString,long> ProfileNameToID;
    _STL::list<CProfileTreeEntry> ProfileTree;
    CProfileTreeEntry* CurrentTreeEntry;
    bool NeedsSaving;
};
FABLE_STATIC_ASSERT(sizeof(CProfileManager::CProfileTreeEntry)==0x60);
FABLE_STATIC_ASSERT(sizeof(CProfileManager)==0x78);
extern CProfileManager* g_FableProfileManager_013CB4F8;

// Ego_r.pdb EDriveReturn (CDriveManager's result code).
enum EDriveReturn
{
    DR_ERROR_UNKNOWN = 0,
    DR_OK = 1,
    DR_ERROR_PATH_NOT_FOUND = 2,
    DR_ERROR_DRIVE_NOT_FOUND = 3,
    DR_ERROR_DIRECTORY_NOT_FOUND = 4,
    DR_ERROR_PATH_ALREADY_EXISTS = 5,
    DR_ERROR_PATH_IS_FILE = 6,
    DR_ERROR_PATH_IS_DIRECTORY = 7
};

// ?PClass@?$CTBaseSingletonNoDefaultAlloc@VCDriveManager@@@@0V?$CCountedPointer@VCDriveManager@@@@A
// is 0x13CAA28. Retail's CDriveManager() stores only the derived vtable.
template<typename T>
class CTBaseSingletonNoDefaultAlloc
{
public:
    static void FABLE_FASTCALL Release()
    {
        PClass.ReleaseData();
    }

    static void FABLE_FASTCALL Set(T* instance)
    {
        PClass.Reset(instance);
    }

    static T& FABLE_FASTCALL Get()
    {
        return *PClass.Data;
    }

protected:
    CTBaseSingletonNoDefaultAlloc()
    {
    }

public:
    virtual ~CTBaseSingletonNoDefaultAlloc()
    {
    }

private:
    static CCountedPointer<T> PClass;
};

// Vtable 0x129AFF4 (RTTI CDriveManager : CTBaseSingletonNoDefaultAlloc<>).
class CDriveManager : public CTBaseSingletonNoDefaultAlloc<CDriveManager>
{
public:
    CDriveManager()
    {
    }

    virtual ~CDriveManager();

    // 0x9E4AD0.
    ESystemCode Initialise(CSystemManager* systemManager);

    // FableWin ?DeleteFileW@CDriveManager@@QAE?AW4EDriveReturn@@ABVCWideString@@@Z
    // (retail 0x009E4BC0; the source's DeleteFile went through <windows.h>'s
    // DeleteFile -> DeleteFileW macro). Caller: CWorld::DeleteCheckPointSave 0x0049E740.
    EDriveReturn DeleteFileW(const CWideString& pathName);
    // FableWin ?DeleteDirectory@CDriveManager@@QAE?AW4EDriveReturn@@ABVCWideString@@@Z, retail 0x009E4B70.
    EDriveReturn DeleteDirectory(const CWideString& pathName);
    // FableWin ?CreateDirectoryW@CDriveManager@@QAE?AW4EDriveReturn@@ABVCWideString@@@Z, retail 0x009E4DA0.
    EDriveReturn CreateDirectoryW(const CWideString& pathname);
    // FableWin ?DriveExists@CDriveManager@@QAE?AW4EDriveReturn@@_W@Z, retail 0x009E4AE0.
    EDriveReturn DriveExists(wchar_t drive);

private:
    // FableWin ?GetDriveFromPath@CDriveManager@@AAE_WPB_W@Z, retail 0x009E4C70.
    wchar_t GetDriveFromPath(const wchar_t* pathname);
    // FableWin ?GetItemFromPath@CDriveManager@@AAE_NPB_WKPA_W@Z, retail 0x009E4CB0.
    bool GetItemFromPath(const wchar_t* pathname, unsigned long directory_number, wchar_t* directory_name);
    CSystemManager* SystemManager;
};

FABLE_STATIC_ASSERT(sizeof(CInputManagerDX) == 0x84);
FABLE_STATIC_ASSERT(sizeof(CSoundManagerLH) == 0x80);
FABLE_STATIC_ASSERT(sizeof(CFontManager) == 0x3C);
FABLE_STATIC_ASSERT(sizeof(CGraphicBankManager) == 0x08);
FABLE_STATIC_ASSERT(sizeof(CMeshBankManager) == 0x0C);
FABLE_STATIC_ASSERT(sizeof(CProfileManager) == 0x78);
FABLE_STATIC_ASSERT(sizeof(CDriveManager) == 0x08);
FABLE_STATIC_ASSERT(sizeof(CDisplayManager) == 0x2C8);
