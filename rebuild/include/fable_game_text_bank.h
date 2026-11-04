#pragma once
class CDataInputStream;

#include "rebuild_abi.h"
#include "fable_counted_pointer.h"
#include "fable_base_class.h"
#include "fable_string.h"
#include "fable_packed_uint_array.h"
#include "fable_checksum_cache_file.h"
#include "fable_vector_map.h"
#include "fable_bank_owned_map.h"
#include "fable_bank_owned_set.h"

class CCharString;
class CWideString;

#include "fable_resource_list.h"
#include "fable_stlport_list.h"
#include "fable_stlport_tree.h"

#include "fable_carray.h"

#include "fable_iv_counted_pointer.h"


class CResourceBank : public CBaseClassNonCopyable
{
public:
    CResourceBank();
    virtual ~CResourceBank();
    void AddResource(CResource&);
    // Ego_r ?RemoveResource@CResourceBank@@QAEXAAVCResource@@@Z, retail 0x009FC3C0.
    void RemoveResource(CResource& resource);
    void AddToLoadingQueue(CResource&);
    void FrameEnd();
    void ProcessLoadingQueue(bool,double);
    static bool FABLE_FASTCALL IsResourceAbortingEnabled(){return ResourceAbortingEnabled;}
    static void FABLE_FASTCALL SetResourceAbortingEnabled(bool enabled){ResourceAbortingEnabled=enabled;}
private:
    static bool ResourceAbortingEnabled;
public:

    CResourceList ResourceList;
    CResourceList LoadingQueue;
};

class CDiskFileWin32;
class CBankFileEntryUpdateData;
class CBankStateBlock;
class CBankFileDataTypeCount;
class CDataOutputStream;
class CDateAndTime;
class CBankFileEntryStats;
class CBankFile : public CBaseClassNonCopyable
{
    friend class CBankFileAsyncEntry; // inferred access: native SetupRead calls protected ReadData.
public:
    // Ego_r/FableWin: 4-byte nested interface. Retail header logging calls slot 2.
    class CProgressCallback {
    public:
        virtual void NotifyBankUpdateProgress(unsigned long, unsigned long) = 0;
        virtual void NotifyBankUpdateStatus(const CCharString&) = 0;
        virtual void NotifyBankLogMessage(const CCharString&) = 0;
        virtual void NotifyBankWarning(const CCharString&) = 0;
        virtual void NotifyBankStats(const CBankFileEntryStats&) = 0;
    };
    CBankFile();
    virtual ~CBankFile();
    virtual bool OpenReadOnly(const CCharString& bankHandle, unsigned long openFlags);
    virtual void OpenReadWrite(const CCharString& bankHandle,
                               unsigned long openFlags, unsigned long flags);
    virtual bool OpenFileReadOnly(const CWideString& filename,
                                  unsigned long openFlags);
    virtual void OpenFileReadWrite(const CWideString& filename,
                                   unsigned long openFlags, unsigned long flags);
    virtual void Close();

    // Retail vtable 0129B54C, slots 6 through 19.
    virtual void AddEntryMultipleFiles(const CCharString&, unsigned long,
        const CArray<CCharString>&, CCountedPointer<CBankStateBlock>);
    virtual void Update();
protected:
    virtual unsigned long GetDataFormatVersion() = 0;
    virtual unsigned long AdjustUpdateEntries();
    virtual void NotifyBankSize(unsigned long, const CArray<CBankFileDataTypeCount>&);
    virtual void NotifyBankOpened();
    virtual void NotifyEntryAdded(unsigned long, unsigned long, const CArray<char>&);
    virtual void NotifyEntryUpdated(unsigned long, unsigned long, const CArray<char>&);
    virtual void NotifyEntryRemoved(unsigned long);
    virtual void NotifyPostEntriesLoaded();
    virtual void PrepareDataForInclusion(unsigned long, const CArray<CCharString>&,
        unsigned long, CCountedPointer<CBankStateBlock>, CDataOutputStream&, CDataOutputStream&);
    virtual bool IsDiskFile(unsigned long, unsigned long);
    virtual CDateAndTime GetLastModifiedTime(const CCharString&);
    virtual CCharString GetEnumerationType(unsigned long) const;
public:


    // FableWin ?CreateSymbolMap@CBankFile@@QBEXAAVCSymbolMap@@_N@Z
    // (retail 0x009CC530; CParticleEmitterDatabase::OpenBank 0x00AE8980).
    void CreateSymbolMap(class CSymbolMap& symbolMap, bool flag) const;

    unsigned long FindIndexBySymbol(const CCharString& symbol) const;
    CCharString GetEntrySymbol(unsigned long index) const;
    unsigned long FindIndexBySymbolCRC(unsigned long crc) const;
    unsigned long GetEntryDataType(unsigned long index) const;
    unsigned long GetEntryDataSize(unsigned long index) const;
    unsigned long GetEntryDataOffset(unsigned long index) const; // retail 009CC270
    struct CompareDataOffset; // Ego_r: one CBankFile* BankFile; orders BlockOrder.
    CWideString GetBankPathname() const; // ?GetBankPathname@CBankFile@@QBE?AVCWideString@@XZ, retail 009CBF10
    CCharString GetBankHandle() const; // retail 009CBA40
    // Ego_r ?SplitSymbol@CBankFile@@QAEXABVCCharString@@PAV2@1@Z, retail 009CBAA0;
    // ?IsSymbolPublic@CBankFile@@QBE_NABVCCharString@@@Z, retail 009CBDF0.
    void SplitSymbol(const CCharString& symbol, CCharString* mainsymbol, CCharString* subscript);
    bool IsSymbolPublic(const CCharString& symbol) const;
private:
    bool ReadFileInformation(bool enable_exceptions); // retail 009D0880, called by read-only open.
    void InitEntry(unsigned long index,const CCharString& symbol,const CCharString& filename);
    void AllocateBuffers(unsigned long size); // retail 009CEAE0.
    bool ReadRecoveryFile(unsigned long validate); // retail 009D0380.
    void InitBlockOrder(); // ?InitBlockOrder@CBankFile@@AAEXXZ, retail 009D0630.
    void PackRuntimeData(); // ?PackRuntimeData@CBankFile@@AAEXXZ, retail 009CD740.
    bool ReadEntryList(class CFileDataInputStream&, unsigned long max_index, unsigned long entry_count);
    void MakeWritable(); // ?MakeWritable@CBankFile@@AAEXXZ, retail 009CBF90
    // Ego_r ?ConvertPathToFilename@CBankFile@@ABE?AVCCharString@@ABV2@@Z, retail 009CBE10.
    CCharString ConvertPathToFilename(const CCharString& file_path) const;
    // Ego_r ?GetRecoveryFilePath@CBankFile@@AAE?AVCWideString@@XZ, retail 009CBFE0;
    // ?DeleteRecoveryFile@CBankFile@@AAEXXZ, retail 009CC160.
    CWideString GetRecoveryFilePath();
    void DeleteRecoveryFile();
public:
protected:
    // Ego_r mangling ?ReadData@CBankFile@@IBEXKPAXK@Z (protected const); retail 009CBF60.
    void ReadData(unsigned long offset, void* data, unsigned long size) const;
public:
    void ReadEntryData(unsigned long index,unsigned long offset,void* data,unsigned long size);
    long GetMaxEntryIndex() const;
    unsigned long GetAlignment() const;
    unsigned long Align(unsigned long size) const;
    bool EntryExists(unsigned long index) const;
    unsigned long CountEntries() const; // retail 009CC3B0
    unsigned long GetTotalDataSize() const; // retail 009CC450
    unsigned long GetUnusedSpace() const; // retail 009CC4C0
    bool EntrySetUpdate(unsigned long index, bool update);

    // Retail CBankFile ends at +0x110. CBankFileAsync owns the following
    // 0x54 bytes; both bank kinds below put CResourceBank at +0x164.
    unsigned long Size;
    // Retail uses four 12-byte STLport arrays (009CD480, 009CD850).
    struct CRuntimeData {
        unsigned long DataOffset, DataSize;
        unsigned char DataType;
        bool Valid;
        unsigned char Padding0A[2];
    };
    CArrayVector<CCharString> Symbols;
    CArrayVector<unsigned long> Checksums;
    CArrayVector<CRuntimeData> RuntimeData;
    CArrayVector<CBankFileEntryUpdateData*> UpdateData;
    CPackedUIntArray PackedDataOffset, PackedDataSize, PackedDataType, PackedValid;
    unsigned long OpenFlags;
    CCountedPointer<CDiskFileWin32> BankFile;
    CWideString Filename;
    CCharString BankHandle;
    bool RetailMode;
    // Ego_r.pdb names (Ego_r offsets +0x10: its CArray is 16 bytes, retail 12).
    bool ReadOnly, WrittenToFile, Solid;
    unsigned long ChangeCount;
    bool FileValid, UpdatingFlag;
    fable_u8 _pad_96[2];
    unsigned long Alignment;
    unsigned long BankFormatVersion;
    unsigned long HeaderSize;
    unsigned long FileInformationSize;
    unsigned long FileSize;
    FableBankOwnedMap<CCharString, unsigned long> SymbolIndices;
    FableBankOwnedMap<CCharString, unsigned long> OldSymbolIndices;
    FableBankOwnedMultiMap<CCharString, unsigned long> FilenameIndices;
    CVectorMap<unsigned long, unsigned long,
               CKeyPairCompareLess<unsigned long, unsigned long> > SymbolCRCIndices;
    _STL::vector<unsigned long> BlockOrder;
    void* ProgressCallback;
    CCharString HeaderEnumerationType;
    // Ego_r.pdb CBankFile::WarningFlag (set by WriteBankWarning 0x009CBD90).
    bool WarningFlag;
    fable_u8 _pad_f5[3];
    CChecksumCacheFile ChecksumCache;
};
FABLE_STATIC_ASSERT(sizeof(CBankFile) == 0x110);

// Retail 009CC640 (out of line): entry indices ordered by their data offset, read
// the same way as GetEntryDataOffset.
struct CBankFile::CompareDataOffset
{
    CBankFile* BankFile;
    CompareDataOffset(CBankFile* bank) : BankFile(bank) {}
    bool operator()(unsigned long a, unsigned long b) const
    {
        unsigned long x = BankFile->PackedDataOffset.Size ? BankFile->PackedDataOffset[a - 1] : BankFile->RuntimeData[a].DataOffset;
        unsigned long y = BankFile->PackedDataOffset.Size ? BankFile->PackedDataOffset[b - 1] : BankFile->RuntimeData[b].DataOffset;
        return x < y;
    }
};
FABLE_STATIC_ASSERT(offsetof(CBankFile, RetailMode) == 0x8c);
FABLE_STATIC_ASSERT(offsetof(CBankFile, Symbols) == 0x08);
FABLE_STATIC_ASSERT(sizeof(CBankFile::CRuntimeData) == 0x0c);

// Retail 009CC240 (out of line, CBankFile_EntryAccessors_009cc210.cpp).
// GetTotalDataSize 009CC450 expands it in place, so its TU defines
// FABLE_BANK_FILE_ENTRY_DATA_SIZE_INLINE for an inline copy of the same body.
#ifdef FABLE_BANK_FILE_ENTRY_DATA_SIZE_INLINE
inline unsigned long CBankFile::GetEntryDataSize(unsigned long index) const
{
    if (PackedDataSize.Size) return PackedDataSize[index - 1];
    return RuntimeData[index].DataSize;
}
#endif
FABLE_STATIC_ASSERT(offsetof(CBankFile, Checksums) == 0x14);
FABLE_STATIC_ASSERT(offsetof(CBankFile, RuntimeData) == 0x20);
FABLE_STATIC_ASSERT(offsetof(CBankFile, UpdateData) == 0x2c);
FABLE_STATIC_ASSERT(offsetof(CBankFile, PackedDataOffset) == 0x38);
FABLE_STATIC_ASSERT(offsetof(CBankFile, SymbolIndices) == 0xac);
FABLE_STATIC_ASSERT(offsetof(CBankFile, ChecksumCache) == 0xf8);

// Retail CBankFileAsync::ConstructorInitialize (0x009D5F80) starts with the
// CBankFile constructor, then initializes the async state at +0x110.
class CBankFileAsyncEntry;
class CBankFileAsyncData;
class CThreadedFile;
class CMovableResourceMemoryPool;
#include "fable_loading_memory_failure_handler.h"
class CBankFileAsync : public CBankFile
{
public:
    static void FABLE_FASTCALL SetEnableFailureHandling(bool);
    CBankFileAsync();
    virtual ~CBankFileAsync();
    virtual bool OpenReadOnly(const CCharString&, unsigned long);
    virtual void OpenReadWrite(const CCharString&, unsigned long, unsigned long);
    virtual bool OpenFileReadOnly(const CWideString&, unsigned long);
    virtual void OpenFileReadWrite(const CWideString&, unsigned long, unsigned long);
    virtual void Close();
    CCountedPointer<CBankFileAsyncEntry> GetAsyncEntry(unsigned long index);
    void AsyncUpdatePending();
    void ProcessDisposalList();
    void DiscardAsyncData(CCountedPointer<CBankFileAsyncData>&);
    void SetLoadingMemoryPool(CCountedPointer<CMovableResourceMemoryPool>);
    bool HandleFailedAllocation(); // retail 009D5BF0
    void SetLoadingMemoryFailureHandler(CCountedPointer<CLoadingMemoryFailureHandler>); // retail 009D66A0
    // Ego_r ?SetNonCached@CBankFileAsync@@QAEX_N@Z; empty in retail (0x009D5230, `ret 4`).
    void SetNonCached(bool);
private:
    // PDB private name; retail 009D5A90 walks ReadsToRestart (+0x134).
    void RestartAbortedReads();
    // WaitUntilFinished (009D5B60) restarts aborted reads under the bank lock.
    friend class CBankFileAsyncData;
    friend struct CBankFileAsyncRestartTestAccess;
public:
    CCountedPointer<CThreadedFile> ThreadedFile;
    CCountedPointer<CMovableResourceMemoryPool> LoadingMemoryPool;
    CCountedPointer<CLoadingMemoryFailureHandler> MemoryFailureHandler;
    FableBankOwnedSet<CBankFileAsyncData*> AsyncDataInstances;
    FableBankOwnedSet<CBankFileAsyncData*> ReadsToRestart;
    CBankFileAsyncData* LastRestartingData;
    _STL::list<CCountedPointer<CBankFileAsyncData> > DisposalList;
    bool NonCached;
    bool DisableAsync;
    fable_u8 _pad_14a[2];
    fable_u8 CriticalSection[0x18];
};
FABLE_STATIC_ASSERT(offsetof(CBankFile, WrittenToFile) == 0x8e);
FABLE_STATIC_ASSERT(offsetof(CBankFile, ChangeCount) == 0x90);
FABLE_STATIC_ASSERT(offsetof(CBankFile, Alignment) == 0x98);
FABLE_STATIC_ASSERT(offsetof(CBankFile, WarningFlag) == 0xf4);
FABLE_STATIC_ASSERT(sizeof(CBankFileAsync) == 0x164);
FABLE_STATIC_ASSERT(offsetof(CBankFileAsync, ThreadedFile) == 0x110);
FABLE_STATIC_ASSERT(offsetof(CBankFileAsync, AsyncDataInstances) == 0x128);
FABLE_STATIC_ASSERT(offsetof(CBankFileAsync, ReadsToRestart) == 0x134);
FABLE_STATIC_ASSERT(offsetof(CBankFileAsync, DisposalList) == 0x144);
FABLE_STATIC_ASSERT(offsetof(CBankFileAsync, CriticalSection) == 0x14c);

#include "fable_threaded_file_priority.h"
class CThreadedFileCallback : public CBaseClassNonCopyable {
public:
    virtual bool OnFileReadBegin(void**, EThreadedFilePriority);
    virtual void OnFileReadCompletion(bool);
};
#include "fable_movable_resource.h"
class CThreadedFileObjectDesc {
public:
    // Recovered default initialization; alignment padding is not written.
    CThreadedFileObjectDesc()
        : PDst(0), FilePos(0), Size(0), PCallback(0),
          QueueWhenFinished(true), Priority(THREADED_FILE_PRIORITY_NORMAL) {}
    void* PDst;
    unsigned long FilePos, Size;
    CThreadedFileCallback* PCallback;
    bool QueueWhenFinished;
    EThreadedFilePriority Priority;
};
class CBankFileAsyncData : public CThreadedFileCallback, public CMovableResource
{
private:
    // Inferred access required by recovered Entry auto-allocation;
    // original friendship breadth is not established by PDB evidence.
    friend class CBankFileAsyncEntry;
    // HandleFailedAllocation (009D5BF0) inlines the private FreeBuffer.
    friend class CBankFileAsync;
    void StartRead(); // retail 009D5650; private void thiscall in both donor PDBs.
    // ??0CBankFileAsyncData@@AAE@PAXKAAVCBankFileAsync@@@Z, retail 009D5D30: caller-supplied buffer.
    CBankFileAsyncData(void*, unsigned long, CBankFileAsync&);
    CBankFileAsyncData(unsigned long, unsigned long, CMovableResourceMemoryPool*, CBankFileAsync&);
    bool SetupBuffer(bool);
    void FreeBuffer(); // ?FreeBuffer@CBankFileAsyncData@@AAEXXZ, retail 009D5410
public:
    virtual ~CBankFileAsyncData();
    virtual bool OnFileReadBegin(void**, EThreadedFilePriority);
    virtual void OnFileReadCompletion(bool);
    virtual void RelocateData(long, void*, void*, long);
    virtual bool CanRelocateData(long) const;
    bool IsFinished() const;
    void* GetData();
    static void FABLE_FASTCALL DiscardAsyncData(CCountedPointer<CBankFileAsyncData>&);
    bool Abort();
    // Ego_r72A2F0 / retail9D5B60; BeginLoading discards the boolean result.
    bool WaitUntilFinished();
    void ChangeLoadingPriority(EThreadedFilePriority); // retail009D5630; public void thiscall in both donor PDBs.
    unsigned long Sizeof() const;
    void* Data;
    void* Buffer;
    unsigned long Length, SkipBytes, ThreadedFileUID;
    CMovableResourceMemoryPool* MemoryPool;
    long MemoryHandle;
    unsigned long InternalAllocatedBufferSize;
    CBankFileAsync& BankFile;
    CThreadedFileObjectDesc LoadDesc;
    volatile bool Finished;
    volatile bool BufferLocked;
    mutable volatile bool AllowRestart;
    bool Aborted;
};
FABLE_STATIC_ASSERT(sizeof(CThreadedFileObjectDesc)==0x18);
namespace _STL {
template<> void list<CCountedPointer<CBankFileAsyncData> >::push_back(const CCountedPointer<CBankFileAsyncData>&);
}
FABLE_STATIC_ASSERT(sizeof(CBankFileAsyncData)==0x48);
FABLE_STATIC_ASSERT(offsetof(CBankFileAsyncData, Data)==0x08);
FABLE_STATIC_ASSERT(offsetof(CBankFileAsyncData, InternalAllocatedBufferSize)==0x24);
// VC7.1 offsetof cannot name a reference member; adjacent fields bound its slot.
FABLE_STATIC_ASSERT(offsetof(CBankFileAsyncData, LoadDesc)==0x2c);
FABLE_STATIC_ASSERT(offsetof(CBankFileAsyncData, Finished)==0x44);
FABLE_STATIC_ASSERT(offsetof(CBankFileAsyncData, AllowRestart)==0x46);

namespace NGameText
{
    class CDataBank;
    class CDataBankEntry
    {
    };
    class CDataBankTextEntry;
    class CDataBankGroupEntry;

    // A loaded group; PFileData stays set while its bank read is in flight.
    class CGroupEntry : public CResource
    {
    public:
        CGroupEntry() {}
        CGroupEntry(const CCountedPointer<CBankFileAsyncData>& data,CDataBankGroupEntry* owner);
        void ReadFromDataStream(CDataInputStream& stream);
        unsigned long Sizeof() const;
        virtual bool UnloadResource();
        bool IsLoaded() const
        {
            return PFileData.Data == 0 || PFileData.Data->IsFinished();
        }

        CArrayVector<long> Entries;
        CCountedPointer<CBankFileAsyncData> PFileData;
        CDataBankGroupEntry* Owner;
    };

    class CDataBankGroupEntry : public CDataBankEntry
    {
    public:
        bool IsLoaded() const
        {
            return !PGroup.IsNull() && PGroup->IsLoaded();
        }

        CIVCountedPointer<CGroupEntry> PGroup;

    private:
        friend class CDataBank;

        void BeginLoading(unsigned long index, CDataBank& bank) const;
        void BuildGroup() const;
    };

    class CTextTag
    {
    public:
        long Pos;
        CCharString Name;
    };

    class CTextEntry : public CResource
    {
    public:
        CTextEntry() {}
        CTextEntry(const CCountedPointer<CBankFileAsyncData>& data,CDataBankTextEntry* owner);
        void ReadFromDataStream(CDataInputStream& stream);
        unsigned long Sizeof() const;
        virtual bool UnloadResource();
        CWideString GetText() const;
        bool IsLoaded() const
        {
            return PFileData.Data == 0 || PFileData.Data->IsFinished();
        }

        CCharString SpeechBank, Narrator, SoundName;
        CWideString Text;
        CArrayVector<CTextTag> Tags;
        CCountedPointer<CBankFileAsyncData> PFileData;
        CDataBankTextEntry* Owner;
    };

    class CDataBankTextEntry : public CDataBankEntry
    {
    public:
        bool IsLoaded() const
        {
            return !PText.IsNull() && PText->IsLoaded();
        }
        CIVCountedPointer<CTextEntry> PText;
        long NarratorIndex;

    private:
        friend class CDataBank;
        void BeginLoading(unsigned long index, CDataBank& bank) const;
        void BuildText() const;
    };

    class CDataBank : public CBankFileAsync, public CResourceBank
    {
    public:
        CDataBank();
        virtual ~CDataBank();
        virtual void Close();

        // Without `load`, an unloaded group only starts loading and null is returned.
        CIVCountedPointer<const CGroupEntry> GetPGroupEntry(
            unsigned long index,
            bool load) const;
        void BeginLoadingGroup(unsigned long index) const;
        void BeginLoadingText(unsigned long index) const;

        const CDataBankGroupEntry* GetGroupInfo(unsigned long index) const
        {
            if (GetEntryDataType(index) != 1)
                return 0;
            return static_cast<const CDataBankGroupEntry*>(Entries[index]);
        }
        const CDataBankTextEntry* GetTextInfo(unsigned long index) const
        {
            if (GetEntryDataType(index) != 0)
                return 0;
            return static_cast<const CDataBankTextEntry*>(Entries[index]);
        }
        CIVCountedPointer<const CTextEntry> GetPTextEntry(
            unsigned long index,
            bool allowPending) const;

        CArrayVector<CDataBankEntry*> Entries;
        CArrayVector<CDataBankTextEntry> TextEntryBuffer;
        CArrayVector<CDataBankGroupEntry> GroupEntryBuffer;
        CArrayVector<CCharString> Narrators;
        unsigned long Seed;

    protected:
    virtual unsigned long GetDataFormatVersion();
    virtual void NotifyBankSize(unsigned long, const CArray<CBankFileDataTypeCount>&);
    virtual void NotifyEntryAdded(unsigned long, unsigned long, const CArray<char>&);
    virtual CDateAndTime GetLastModifiedTime(const CCharString&);
    virtual CCharString GetEnumerationType(unsigned long) const;
        void CloseTextAndGroups();
    };
}

FABLE_STATIC_ASSERT(sizeof(NGameText::CDataBankTextEntry) == 8);
FABLE_STATIC_ASSERT(sizeof(NGameText::CTextTag) == 8);
FABLE_STATIC_ASSERT(sizeof(NGameText::CTextEntry) == 0x44);
FABLE_STATIC_ASSERT(offsetof(NGameText::CTextEntry, Text) == 0x28);
FABLE_STATIC_ASSERT(offsetof(NGameText::CTextEntry, PFileData) == 0x38);
FABLE_STATIC_ASSERT(offsetof(NGameText::CTextEntry, Owner) == 0x40);
FABLE_STATIC_ASSERT(sizeof(NGameText::CGroupEntry) == 0x34);
FABLE_STATIC_ASSERT(offsetof(NGameText::CGroupEntry, PFileData) == 0x28);
FABLE_STATIC_ASSERT(offsetof(NGameText::CGroupEntry, Owner) == 0x30);
FABLE_STATIC_ASSERT(sizeof(NGameText::CDataBankGroupEntry) == 4);
FABLE_STATIC_ASSERT(sizeof(NGameText::CDataBank) == 0x214);
FABLE_STATIC_ASSERT(offsetof(NGameText::CDataBank, Entries) == 0x1e0);
FABLE_STATIC_ASSERT(offsetof(NGameText::CDataBank, TextEntryBuffer) == 0x1ec);
FABLE_STATIC_ASSERT(offsetof(NGameText::CDataBank, GroupEntryBuffer) == 0x1f8);
FABLE_STATIC_ASSERT(offsetof(NGameText::CDataBank, Narrators) == 0x204);
FABLE_STATIC_ASSERT(offsetof(NGameText::CDataBank, Seed) == 0x210);

namespace NLocalisation
{
    // ?GetTextBankName@NLocalisation@@YI?AVCCharString@@XZ (retail 0x00415260;
    // caller CNewFrontendGameComponent::Init2 0x0042E98F).
    CCharString FABLE_FASTCALL GetTextBankName();

    // ?GetLanguageFolder@NLocalisation@@YI?AVCCharString@@XZ (retail 0x00415530;
    // callers: the NResourceDirectoryNames language getters, e.g.
    // GetDVDDialogueDir 0x0041A2D0).
    CCharString FABLE_FASTCALL GetLanguageFolder();
}
