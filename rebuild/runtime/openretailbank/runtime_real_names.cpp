// Real C++ names for the host stand-ins in runtime_helpers.cpp.
//
// The de-baked leaves 009a9c80 and 009ac530 now call the retail members by
// their real names instead of the old Fable*_009A9040 / _009AC530 adapters.
// These thin definitions route those names to the existing host behaviour,
// so the standalone link keeps the same semantics as before the de-bake.

#include <stdio.h>

#include "fable_data_input_stream.h"
#include "fable_disk_file.h"
#include "fable_retail_banks.h"
#include "fable_stlport_vector.h"
#include "fable_threaded_file.h"

extern "C" void FABLE_FASTCALL
FableThreadedFileObjectDelete_009A9040(void* object);
extern "C" void FABLE_FASTCALL
FableContainedBankMapInsert_009AC530(
    void* mapValue, void*, void** result, void*, const void* value);
extern "C" void FABLE_FASTCALL
FableContainedBankMapStringCopy_009AC530(
    void* destination, void*, const void* source);
extern "C" bool FABLE_FASTCALL
FableContainedBankMapStringLess_009AC530(const void*, void*, const void*);
extern "C" void* FABLE_FASTCALL
FableContainedBankMapLowerBound_009AC530(void* mapValue, void*, const void*);

void FABLE_FASTCALL CCountedPointer<CThreadedFile>::DeleteData(void* data)
{
    FableThreadedFileObjectDelete_009A9040(data);
}

CCharString::CCharString(const CCharString& other)
{
    FableContainedBankMapStringCopy_009AC530(this, 0, &other);
}

bool CCharStringData::operator<(const CCharStringData& other) const
{
    return FableContainedBankMapStringLess_009AC530(this, 0, &other);
}

NBankFileManager::CContainedBankMap::CNode*
NBankFileManager::CContainedBankMap::LowerBound(const CCharString& key) const
{
    return static_cast<CNode*>(FableContainedBankMapLowerBound_009AC530(
        const_cast<CContainedBankMap*>(this), 0, &key));
}

NBankFileManager::CContainedBankMap::iterator
NBankFileManager::CContainedBankMap::InsertUnique(
    iterator, const value_type& value)
{
    void* node = 0;
    FableContainedBankMapInsert_009AC530(this, 0, &node, 0, &value);
    return iterator(static_cast<CNode*>(node));
}

// ---------------------------------------------------------------------------
// Real names the genuine OpenRetailBank (0x9A8840) calls. The old bake called
// these through the FableOpenRetail*_009A8840 thunks in ring_thunks.cpp; the
// same ring targets now bind to the real names, so the module keeps the
// behaviour the probe verified. Each /alternatename only applies while the
// real name is otherwise unresolved.

// CAFile::PathExists 0x999230 -> the host stand-in (always true).
#pragma comment(linker, "/alternatename:?PathExists@CAFile@@SI_NABVCWideString@@@Z=_FableHostPathExists_009A8840")
// The inline CDiskFileWin32() calls the base ctor; retail's is 0x99A2F0.
#pragma comment(linker, "/alternatename:??0CAFile@@QAE@XZ=??0CBaseClassNonCopyable@@QAE@XZ")
// The faithful bank constructor still spells the root class "CBase"; it is CBaseClass
// (retail RTTI .?AVCBaseClass@@, ctor 0x0099A2D0).
#pragma comment(linker, "/alternatename:??0CBase@@QAE@XZ=??0CBaseClass@@QAE@XZ")
// CFileDataInputStream ctor 0x994700 / dtor 0x994780 (ring reconstructions).
#pragma comment(linker, "/alternatename:??0CFileDataInputStream@@QAE@PAVCAFile@@K@Z=@CFileDataInputStream_CFileDataInputStream_00994700@16")
#pragma comment(linker, "/alternatename:??1CFileDataInputStream@@UAE@XZ=@CFileDataInputStream_dtor_00994780@8")
// CDataInputStream::ReadWithSrcChunkOverflow 0x993CA0 -> host refill reader.
#pragma comment(linker, "/alternatename:?ReadWithSrcChunkOverflow@CDataInputStream@@QAEXPAXJ@Z=@FableHostReadOverflow_009A8840@16")
// CDataInputStream::SetPosition 0x993BC0: the landing now defines the real
// virtual symbol, so no alias is needed.
// vector<unsigned char>(n) 0x411910 (faithful zero-filled allocation).
#pragma comment(linker, "/alternatename:??0?$vector@EV?$allocator@E@_STL@@@_STL@@QAE@I@Z=?Buffer_AllocateAndZero@@YIPAUBufferOverlay@@PAU1@PAXI@Z")
// CMemoryDataInputStream(const void*, ulong) 0x994930, its dtor 0x993B10,
// ReadNullTerminatedString 0x995D30.
#pragma comment(linker, "/alternatename:??0CMemoryDataInputStream@@QAE@PBXK@Z=@FableHostMemoryStreamConstruct@16")
// The 0x00993B10 / 0x00995D30 landings now provide the real virtual symbols.
// CWideString::ToCharString 0x99B2C0, CCharString::operator= 0x99EFB0.
#pragma comment(linker, "/alternatename:?ToCharString@CWideString@@QBE?AVCCharString@@XZ=?ConvertToCharString@CWideString@@QBE?AVCCharString@@XZ")
#pragma comment(linker, "/alternatename:??4CCharString@@QAEAAV0@ABV0@@Z=?CCharString_operator_assign@@YIPAUCCharString@@PAU1@PAX0@Z")
// CThreadedFile ctor 0x98DFD0 (the ring's landed copy).
#pragma comment(linker, "/alternatename:??0CThreadedFile@@QAE@XZ=@CThreadedFile_ctor@4")

extern "C" bool FABLE_FASTCALL FableHostDiskOpen(
    void*, void*, const void*, unsigned long, unsigned long);
extern "C" long FABLE_FASTCALL FableHostDiskGetPosition(void*);
extern "C" void FABLE_FASTCALL FableHostDiskRead(void*, void*, void*, long);

// The disk file the anchor now constructs itself (inline CDiskFileWin32())
// has the retail CDiskFileWin32 layout, which the host disk helpers already
// use; its virtuals route to them.
void CDiskFileWin32::Open(
    const CWideString& pathname,
    FileAccessType accessType,
    unsigned long flags)
{
    FableHostDiskOpen(this, 0, &pathname, accessType, flags);
}

void CDiskFileWin32::Close()
{
    FILE* file = static_cast<FILE*>(WinFileHandle);
    if (Opened && file && file != (FILE*)-1)
        fclose(file);
    WinFileHandle = 0;
    Opened = false;
}

void CDiskFileWin32::Read(void* data, long size, CASuspendableProcess*)
{
    FableHostDiskRead(this, 0, data, size);
}

void CDiskFileWin32::Write(const void*, long, CASuspendableProcess*) {}

void CDiskFileWin32::SetPosition(long position)
{
    FILE* file = static_cast<FILE*>(WinFileHandle);
    if (file && fseek(file, position, SEEK_SET) == 0)
        Position = position;
}

void CDiskFileWin32::MovePosition(long offset)
{
    SetPosition(Position + offset);
}

long CDiskFileWin32::GetPosition() const
{
    return FableHostDiskGetPosition(const_cast<CDiskFileWin32*>(this));
}

void CDiskFileWin32::SetLength(long) {}
CAFile* CDiskFileWin32::Clone() const { return 0; }

class CFileInformation
{
};

CFileInformation CDiskFileWin32::GetFileInformation()
{
    return CFileInformation();
}

// Ring stand-in for FableWin ?GetFileInformation@CAFile@@UAE?AVCFileInformation@@XZ
// (CAFile's own vtable slot; not reconstructed yet). The bank path never calls it.
CFileInformation CAFile::GetFileInformation()
{
    return CFileInformation();
}

CWideString::CWideString(const CWideString& other)
    : storage_(other.storage_)
{
}

void FABLE_FASTCALL CCountedPointer<CDiskFileWin32>::DeleteData(void* data)
{
    delete static_cast<CDiskFileWin32*>(data);
}

// The host CRetailBank (faithful 0x9AC4D0) has no real destructor behind its
// vtable; release only the allocation.
void FABLE_FASTCALL
CCountedPointer<NBankFileManager::CRetailBank>::DeleteData(void* data)
{
    operator delete(data);
}
