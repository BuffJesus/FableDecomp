#pragma once

#include "fable_threaded_file_priority.h"
#include "fable_threaded_file_device.h"
#include "fable_base_class.h"
#include "fable_stlport_tree.h"

#include "fable_counted_pointer.h"
#include "fable_system.h"

#include "fable_init_base_class.h"

#include "fable_system_manager_state.h"

class CBankFileManager
{
public:
    void Close();
    // FableWin ?OpenRetailBank@CBankFileManager@@QAEXABVCWideString@@_N@Z (0x009A8840)
    // and ?CloseRetailBank@CBankFileManager@@QAEXABVCWideString@@@Z (0x009A78D0).
    void OpenRetailBank(const CWideString& pathname, bool startupPriority);
    void CloseRetailBank(const CWideString& pathname);
};

struct _OVERLAPPED;
class CThreadedFile;
class CThreadedFileObjectDesc;

// Retail PC layout (sizeof 0x178; vtable 0x0129A164, ctor 0x0098F3A0, dtor 0x0098E690).
// PDB Ego_r member names/order; STLport maps are 12 bytes (PDB build used 16-byte maps).
class CThreadedFileManager : public CBaseClassNonCopyable
{
public:
    CThreadedFileManager(); // retail 0098F3A0
    typedef void (FABLE_FASTCALL* ErrorCallback)();
    typedef void (FABLE_FASTCALL* HardDiskErrorCallback)(const char*);
    // FableWin public static callback policy; retail 00401460 / 00401470.
    static void FABLE_FASTCALL SetErrorCallback(ErrorCallback callback);
    static void FABLE_FASTCALL SetHardDiskErrorCallback(HardDiskErrorCallback callback);
    static void FABLE_FASTCALL SetDirtyDiscError(bool dirty); // 0098DED0
    static void FABLE_FASTCALL DoHardDiskErrorCallback(const char* text); // 0098F9B0
    void Uninitialise();
    // FableWin PDB public methods; retail 0098E170 / 0098E190.
    void SetDeviceTimeSlice(EThreadedFileDeviceID device, float timeSlice);
    void KeepUpdating();
    // FableWin PDB names; retail 0098E390 / 0098E410 / 0098E490 / 0098E510.
    long GetTotalObjectsPendingCount() const;
    long GetTotalBytesPendingCount() const;
    long GetTotalObjectsFinishedCount() const;
    long GetTotalBytesFinishedCount() const;
    unsigned long AddObject(const CThreadedFileObjectDesc& desc, CThreadedFile& file);
    void CloseAllStreamsForFile(CThreadedFile* file);
    bool RemoveObject(unsigned long uid);
    bool IsObjectFinished(unsigned long uid) const; // 0098E590
    bool ObjectExists(unsigned long uid) const; // 0098E5F0
    void WaitForObjectToFinish(unsigned long uid);
    void SetObjectPriority(unsigned long uid, EThreadedFilePriority priority);
    // FableWin ?ProcessDirtyDiscError@CThreadedFileManager@@SIXXZ. Empty on
    // PC (0x0098E1B0, a bare `ret` in the threaded-file TU between
    // Uninitialise 0x0098E0D0 and CThreadedFile::Open 0x0098E1E0); its only
    // caller is CMainGameComponent::Run 0x004189C2 (once per frame).
    static void FABLE_FASTCALL ProcessDirtyDiscError();
    virtual ~CThreadedFileManager();
    // Ego_r S_CONSTANT values.
    enum EFileLoadingStatus
    {
        FILE_LOADING_STATUS_PENDING = 0,
        FILE_LOADING_STATUS_PENDING_LOCKED = 1,
        FILE_LOADING_STATUS_READING = 2,
        FILE_LOADING_STATUS_READ_COMPLETED = 3,
        FILE_LOADING_STATUS_FINISHED = 4
    };
    // Nested types are public so the uid map can be specialised by the manager TU.
    class CFileObject; // fable_threaded_file_object.h (0x40 bytes)
    struct CPendingObjectSortKey
    {
        CPendingObjectSortKey() : Object(0) {} // FableWin0300DBA0
        CPendingObjectSortKey(CFileObject* object) : Object(object) {} // Convenience overload; retail0098F5B0 is the implicit copy constructor.
        CFileObject* Object;
        bool operator<(const CPendingObjectSortKey& other) const; // 0x0098F570
    };
    struct CPendingObjectMap
    {
        // Retail initializes this cursor before the next map allocation.
        CPendingObjectMap();
        _STL::map<CPendingObjectSortKey, CFileObject*> Objects;
        _STL::map<CPendingObjectSortKey, CFileObject*>::iterator CurrentPosition;
    };
protected:
    static HardDiskErrorCallback FABLE_FASTCALL GetHardDiskErrorCallback(); // 0098DEE0
    static void FABLE_FASTCALL CallHardDiskErrorCallback(const char* text); // 0098DEF0
private:
    // FableWin PDB private methods; retail 0098F2A0 / 0098ECD0.
    static void __cdecl BeginThread(void* context); // retail 0098F380
    void ThreadMain();
    void ProcessObjects();
    static void __stdcall OnFileReadCompletion(unsigned long error, unsigned long transferred, _OVERLAPPED* overlapped); // 0098E320
    static void (FABLE_FASTCALL* m_harddiskErrorCallback)(const char*); // 013BC864
    static bool m_bDirtyDiscError; // 013BC868
    static ErrorCallback m_errorCallback; // 013BC860
    void RemoveObjectFromPendingMap(CFileObject* object); // 0x0098E770
    void OnFinishObject(CFileObject* object); // 0x0098EBC0
    void AddObjectToPendingMap(CFileObject* object); // 0x0098ECA0
    CPendingObjectMap DeviceQueue[THREADED_FILE_DEVICE_ID_COUNT][THREADED_FILE_PRIORITY_COUNT]; // +0x004
    EThreadedFileDeviceID CurrentDeviceAtPriorityLevel[THREADED_FILE_PRIORITY_COUNT];           // +0x0E4
    double CurrentDeviceEndOfTimeSlice[THREADED_FILE_PRIORITY_COUNT];                           // +0x100
    CFileObject* CurrentlyReadingObject;                                                        // +0x138
    _STL::map<unsigned long, CFileObject*> Objects;                                              // +0x13C
    bool Initialised;                                                                            // +0x148
    bool ExitThread;                                                                             // +0x149
    bool ThreadFinished;                                                                         // +0x14A
    unsigned long CriticalSection[6];  // RTL_CRITICAL_SECTION storage, 24 bytes, 4-aligned      // +0x14C
    void* Event;                                                                                 // +0x164
    unsigned long NextObjectUID;                                                                 // +0x168
    long BlockPendingObjectProcessingRefCount;                                                   // +0x16C
    float DeviceTimeSlice[THREADED_FILE_DEVICE_ID_COUNT];                                        // +0x170
    friend class CThreadedFileManagerLayout;
};
class CThreadedFileManagerLayout
{
    FABLE_STATIC_ASSERT(sizeof(CThreadedFileManager::CPendingObjectMap) == 0x10);
    FABLE_STATIC_ASSERT(offsetof(CThreadedFileManager, CurrentDeviceAtPriorityLevel) == 0xE4);
    FABLE_STATIC_ASSERT(offsetof(CThreadedFileManager, CurrentlyReadingObject) == 0x138);
    FABLE_STATIC_ASSERT(offsetof(CThreadedFileManager, Objects) == 0x13C);
    FABLE_STATIC_ASSERT(offsetof(CThreadedFileManager, CriticalSection) == 0x14C);
    FABLE_STATIC_ASSERT(offsetof(CThreadedFileManager, DeviceTimeSlice) == 0x170);
    FABLE_STATIC_ASSERT(sizeof(CThreadedFileManager) == 0x178);
};

#include "fable_ime.h"

extern CBankFileManager g_FableBankFileManager_013CA79C;
extern CThreadedFileManager g_FableThreadedFileManager_013BC870;

void FABLE_FASTCALL FableFileInstallerSingletonRelease();
// The retail CSystemManager object (0x013CA618) GFGetSystemManager returns.
extern CSystemManager GSystemManager;
CSystemManager* FABLE_FASTCALL GFGetSystemManager();

// CSystemManager members GFInitialise 0x004022B0 caches in globals.
class CRenderManager2D;
extern CASoundManager* g_FableSoundManager_013B8394;             // PSoundManager
extern CDisplayManager* g_FableDisplayManager_013B8390;          // PDisplayManager
extern CRenderManager2D* g_FableRenderManager2D_013B8384;        // PDisplayManager->PRenderManager
extern CInputManager* g_FableInputManager_013B8388;              // PInputManager (GPInputManager)
extern CFontManager* g_FableFontManager_013B838C;                // PFontManager
extern CGraphicBankManager* g_FableGraphicBankManager_013B837C;  // PGraphicBankManager
void FABLE_FASTCALL UninitializeStreamingFontTextureCache();

void FABLE_CDECL GFUninitialise();
void FABLE_STDCALL GFHandleSystemInitError(unsigned long errorCode);
