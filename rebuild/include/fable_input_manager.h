#pragma once

// CInputManager with its Ego_r members at retail offsets (STLport vectors are
// 12 bytes, so the class is 0x80 where Ego_r's is 0x90). Retail vtable of the
// DX subclass 0x0129D620: scalar deleting destructor, SetExclusiveMode
// 0x009F4E70, Update 0x009F57A0 (FableWin names).

#include "fable_base_class.h"
#include "fable_scoped_ptr.h"
#include "fable_checksum.h"
#include "fable_counted_pointer.h"
#include "fable_disk_file.h"
#include "fable_input_devices.h"
#include "fable_input_event.h"
#include "fable_stlport_vector.h"

class CInputManagerInit;

// Ego_r CVersion (12 bytes): GVersion is the one retail streams.
class CVersion
{
public:
    // Ego_r ??0CVersion@@QAE@EEKK@Z, retail 0x009EA080.
    CVersion(unsigned char day, unsigned char month, unsigned long year, unsigned long build);
    // Ego_r ??8CVersion@@QBE_NABV0@@Z, retail 0x009EA0A0 (absent from the manifest).
    bool operator==(const CVersion& version) const;
    // Ego_r ?GetAsString@CVersion@@QBE?AVCCharString@@XZ, retail 0x009EA0D0 (/GS).
    CCharString GetAsString() const;

    unsigned char Month;
    unsigned char Day;
    unsigned long Year;
    unsigned long Build;
};

// Ego_r CInputEventPackage (0x34; retail 0x30): one frame's recorded inputs.
class CInputEventPackage
{
public:
    explicit CInputEventPackage(CAFile& file);
    void Save(CAFile& file);

    _STL::vector<CInputEvent> Events;                               // +0x00
    long TimeStamp;                                                 // +0x0C
    CChecksum Checksum;                                             // +0x10
    float MousePos[2];                                              // +0x20 C2DVector
    float JoystickPos[2];                                           // +0x28 C2DVector
};

// STLport's erase over CInputEvent is the non-POD form (counted copy, then
// each tail element's destructor), out of line in retail (0x009F67E0;
// ClearEvents empties both event vectors through it).
template <> CInputEvent* _STL::vector<CInputEvent>::erase(CInputEvent* first, CInputEvent* last);

class CInputManager : public CBaseClass
{
public:
    CInputManager(const CInputManagerInit& init, bool exclusiveMode);
    virtual ~CInputManager();
    virtual bool SetExclusiveMode(bool exclusive);                  // 0x009F4E70
    virtual void Update();                                          // 0x009F57A0

    void ClearEvents();                                             // 0x009F5540
    bool EventScanStart(CInputEventScanInfo& info, CInputEvent& event) const;  // 0x009F4ED0
    bool EventScanNext(CInputEventScanInfo& info, CInputEvent& event) const;   // 0x009F4F10
    void EventScanEnd(CInputEventScanInfo& info) const;             // 0x009F4AC0
    // ?UpdateChecksum@CInputManager@@QAEXJABVCChecksum@@@Z (retail 0x009F4A90;
    // caller CMainGameComponent::GetInputs 0x00416E78).
    void UpdateChecksum(long frame, const CChecksum& checksum);

    // FableWin ?StreamInputsToFile / ?StreamInputsFromFile@CInputManager@@
    // QAEXABVCVersion@@ABVCWideString@@@Z (retail 0x009F4CF0 / 0x009F5E10;
    // callers CMainGameComponent::BeginInputSaving 0x00416268 /
    // BeginInputLoading 0x0041627F).
    void StreamInputsToFile(const CVersion& version, const CWideString& fileName);
    void StreamInputsFromFile(const CVersion& version, const CWideString& fileName);
    // FableWin ?IsJoystickDeviceNumberActive@CInputManager@@QBE_NJ@Z (retail
    // 0x009F5250; callers CMainGameComponent::IsControllerDisconnected 0x00416296,
    // CNewFrontendGameComponent::IsControllerDisconnected 0x0042DC63).
    bool IsJoystickDeviceNumberActive(long deviceNumber) const;

    // FableWin ?SetHasInputFocus@CInputManager@@QAEX_N@Z (retail 0x009F4E20;
    // callers CSystemManager::SetHasInputFocus 0x009A5A30 / UpdateActive 0x009A6370).
    void SetHasInputFocus(bool hasFocus);

    // FableWin ?IsLoadingEvents / ?IsSavingEvents@CInputManager@@QBE_NXZ;
    // inline in retail (CMainGameComponent::Update 0x00418289 reads
    // GPInputManager +0x38 / +0x39).
    bool IsLoadingEvents() const
    {
        return Loading;
    }

    bool IsSavingEvents() const
    {
        return Saving;
    }

protected:
    // Ego_r ?EmptyEventStore@CInputManager@@IAEXXZ; ClearEvents expands it.
    void EmptyEventStore()
    {
        EventStore.clear();
        NoStoredEvents = 0;
    }
    // Ego_r ?OpenPackageFileForSaving@CInputManager@@IAEXABVCVersion@@ABVCWideString@@@Z
    // (retail keeps a copy at 0x009F4AD0; StreamInputsToFile expands it). The
    // package file is driven through its CAFile interface (virtual calls).
    void OpenPackageFileForSaving(const CVersion& version, const CWideString& fileName)
    {
        CAFile& file = PackageFile;
        file.Open(fileName, FILE_ACCESS_CREATE, 2);
        file.Write(&version, sizeof(version), 0);
        CurrentFilePackageIndex = 0;
    }
    // Ego_r ?LoadAllEventPackages@CInputManager@@IAEXABVCVersion@@AAVCAFile@@@Z
    // (retail 0x009F5D70).
    void LoadAllEventPackages(const CVersion& version, CAFile& file);

    _STL::vector<CInputEvent> ProcessedEvents;                      // +0x04
    _STL::vector<CInputEvent> EventStore;                           // +0x10
    long NoStoredEvents;                                            // +0x1C
    mutable long NoEventScansRunning;                               // +0x20
    boost::scoped_ptr<CMouse> PMouse;                               // +0x24
    boost::scoped_ptr<CKeyboard> PKeyboard;                         // +0x28
    _STL::vector<CCountedPointer<CJoystick> > PJoysticks;           // +0x2C
    bool Loading;                                                   // +0x38
    bool Saving;                                                    // +0x39
    CChecksum Checksum;                                             // +0x3C
    _STL::vector<CInputEventPackage> LoadedEventPackages;           // +0x4C
    long CurrentFilePackageIndex;                                   // +0x58
    long TimeStamp;                                                 // +0x5C
    CDiskFileWin32 PackageFile;                                     // +0x60
    long NoUpdates;                                                 // +0x7C

    friend struct CInputManagerTestAccess;
};

struct IDirectInput8W;

class CInputManagerDX : public CInputManager
{
public:
    CInputManagerDX(const CInputManagerInit& init, bool exclusiveMode);

    // Ego_r ?GetDInput@CInputManagerDX@@QAEPAUIDirectInput8W@@XZ, inline in retail (the DX devices create and
    // enumerate through it: CJoystickDX::SetExclusiveMode 0x00AB6DE0).
    IDirectInput8W* GetDInput()
    {
        return DInput;
    }

private:
    IDirectInput8W* DInput;                                         // +0x80
};

FABLE_STATIC_ASSERT(sizeof(CVersion) == 0x0C);
FABLE_STATIC_ASSERT(sizeof(CInputEventPackage) == 0x30);
FABLE_STATIC_ASSERT(sizeof(CInputManager) == 0x80);
FABLE_STATIC_ASSERT(sizeof(CInputManagerDX) == 0x84);
