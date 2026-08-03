// Closure helpers for the standalone OpenRetailBank link.  These are kept
// deliberately small: the raw anchor and the 27 reconstructed direct
// callees are the parity surface; these support routines make the structural
// link possible while the real host/filesystem and STL leaves are recovered.

#include <cstdlib>
#include <cstring>
#include <cstdio>

#include "rebuild_abi.h"

#pragma comment(linker, "/alternatename:?UnassignString@CCharString@@QAEXXZ=?UnassignString@CCharString@@AAEXXZ")

int g_CWideStringInstanceCount_013BCA20 = 0;
int g_CCharStringInstanceCount_013BD800 = 0;

extern "C" void* CFileDataInputStream_VTable_00994700[11] = { 0 };
extern "C" void* CFileDataInputStream_BaseVTable_00994780[8] = { 0 };
extern "C" void* CFileDataInputStream_DerivedVTable_00994780[8] = { 0 };
void* g_CChunkedFileChunkVTable_0129a69c[8] = { 0 };

struct CBase
{
    void RestoreVTable();
};

void CBase::RestoreVTable()
{
}

struct CCharString
{
public:
    CCharString(const char* text, long length);
    CCharString(const CCharString& other);

private:
    void UnassignString();
    void* storage_;
};

#pragma pack(push, 1)
struct FableCharDataHost
{
    char* text;
    unsigned long length;
    unsigned long unknown08;
    unsigned char flags0C;
    long owners;
};
#pragma pack(pop)

CCharString::CCharString(const char* text, long length)
    : storage_(0)
{
    if (!text) return;
    if (length < 0) length = (long)std::strlen(text);
    FableCharDataHost* data = (FableCharDataHost*)std::malloc(
        sizeof(FableCharDataHost));
    if (!data) return;
    data->text = (char*)std::malloc((size_t)length + 1);
    if (!data->text) { std::free(data); return; }
    std::memcpy(data->text, text, (size_t)length);
    data->text[length] = 0;
    data->length = (unsigned long)length;
    data->unknown08 = 0;
    data->flags0C = 0;
    data->owners = 1;
    storage_ = data;
}

CCharString::CCharString(const CCharString& other)
    : storage_(other.storage_)
{
    if (storage_)
        ++reinterpret_cast<FableCharDataHost*>(storage_)->owners;
}

void CCharString::UnassignString()
{
    FableCharDataHost* data = reinterpret_cast<FableCharDataHost*>(storage_);
    if (data && --data->owners == 0)
    {
        std::free(data->text);
        std::free(data);
    }
    storage_ = 0;
}

class CWideString
{
public:
    ~CWideString();
    CWideString& operator=(const CWideString& other);
    operator const wchar_t*() const;

private:
    void* storage_;
};

CWideString::~CWideString()
{
    storage_ = 0;
}

CWideString& CWideString::operator=(const CWideString& other)
{
    if (this != &other)
        storage_ = other.storage_;
    return *this;
}

CWideString::operator const wchar_t*() const
{
    if (!storage_) return L"";
    return *reinterpret_cast<const wchar_t* const*>(storage_);
}

class CDataInputStream
{
public:
    CDataInputStream();
    virtual ~CDataInputStream();
};

CDataInputStream::CDataInputStream()
{
}

CDataInputStream::~CDataInputStream()
{
}

class CBasicStringCharShim
{
public:
    bool Equals(const char* literal);
};

bool CBasicStringCharShim::Equals(const char*)
{
    return false;
}

class CCharStringConvertShim
{
public:
    void* ToCharPointer(class CCharStringSlot*, class CCharStringSlot*);
};

class CCharStringSlot;

void* CCharStringConvertShim::ToCharPointer(
    CCharStringSlot*, CCharStringSlot*)
{
    return this;
}

struct FableWideDataHost
{
    wchar_t* text;
    unsigned long unknown04;
    unsigned long unknown08;
    long owners;
};

struct FableDiskHost
{
    void** vtable;
    unsigned long accessType;
    long position;
    long length;
    FILE* file;
    unsigned char opened;
    unsigned char padding15[3];
    void* pathname;
};

extern "C" long FABLE_FASTCALL FableHostBufferedGetPosition(void*);
extern "C" void FABLE_FASTCALL FableHostBufferedReadNext(
    void*, void*, void*, long*);
extern "C" __declspec(naked) bool FableHostBufferedCanRead(void*, long)
{
    __asm
    {
        mov eax, 1
        ret 4
    }
}
extern "C" void FABLE_FASTCALL FableHostBufferedRead(
    void*, void*, void*, long);

struct FableHostRefBlock
{
    long owners;
    void (FABLE_FASTCALL *destroy)(void*);
    void* object;
};

extern "C" void FABLE_FASTCALL FableHostDiskObjectDestroy(void* object)
{
    FableDiskHost* disk = reinterpret_cast<FableDiskHost*>(object);
    if (disk && disk->file && disk->file != (FILE*)-1)
        fclose(disk->file);
    std::free(disk);
}

extern "C" void FABLE_FASTCALL FableHostDiskOwnerConstruct_009A8840(
    void* self, void*, void* object)
{
    void** fields = reinterpret_cast<void**>(self);
    fields[0] = object;
    fields[1] = 0;
    if (!object)
        return;

    FableHostRefBlock* block =
        reinterpret_cast<FableHostRefBlock*>(std::malloc(sizeof(*block)));
    if (!block)
        return;
    block->owners = 1;
    block->destroy = FableHostDiskObjectDestroy;
    block->object = object;
    fields[1] = block;
}

static void* g_FableHostMemoryStreamVTable[3] = { 0, 0, 0 };

extern "C" long FABLE_FASTCALL FableHostMemoryGetPosition(void* self)
{
    return *reinterpret_cast<unsigned long*>(
        reinterpret_cast<unsigned char*>(self) + 4);
}

extern "C" void FABLE_FASTCALL FableHostMemoryStreamConstruct(
    void* self, void*, void* buffer, unsigned long size)
{
    if (!g_FableHostMemoryStreamVTable[2])
        g_FableHostMemoryStreamVTable[2] = (void*)FableHostMemoryGetPosition;
    *reinterpret_cast<void***>(self) = g_FableHostMemoryStreamVTable;
    unsigned char* bytes = reinterpret_cast<unsigned char*>(self);
    *reinterpret_cast<unsigned long*>(bytes + 4) = 0;
    *reinterpret_cast<unsigned long*>(bytes + 8) = size;
    *reinterpret_cast<void**>(bytes + 0x0C) = buffer;
    *reinterpret_cast<unsigned long*>(bytes + 0x10) = 0;
    *reinterpret_cast<unsigned long*>(bytes + 0x14) = size;
    *reinterpret_cast<void**>(bytes + 0x18) = buffer;
}

static const wchar_t* FableHostPathText(const void* pathname)
{
    if (!pathname) return L"";
    FableWideDataHost* data =
        *reinterpret_cast<FableWideDataHost* const*>(pathname);
    return data && data->text ? data->text : L"";
}

extern "C" bool __cdecl FableHostPathExists_009A8840()
{
    return true;
}

extern "C" void* __cdecl FableHostOperatorNew_009A8840(unsigned int size)
{
    return std::malloc(size);
}

extern "C" bool FABLE_FASTCALL FableHostDiskOpen(
    void* self, void*, const void* pathname, unsigned long accessType,
    unsigned long)
{
    FableDiskHost* disk = reinterpret_cast<FableDiskHost*>(self);
    const wchar_t* text = FableHostPathText(pathname);
    FILE* file = _wfopen(text, L"rb");
    if (!file)
    {
        disk->file = 0;
        disk->opened = 0;
        return false;
    }

    fseek(file, 0, SEEK_END);
    long length = ftell(file);
    fseek(file, 0, SEEK_SET);
    disk->accessType = accessType;
    disk->position = 0;
    disk->length = length;
    disk->file = file;
    disk->opened = 1;
    return true;
}

extern "C" long FABLE_FASTCALL FableHostDiskGetPosition(void* self)
{
    return reinterpret_cast<FableDiskHost*>(self)->position;
}

extern "C" unsigned long FABLE_FASTCALL FableHostDiskGetLength(void* self)
{
    return (unsigned long)reinterpret_cast<FableDiskHost*>(self)->length;
}

extern "C" void FABLE_FASTCALL FableHostDiskReadNext(
    void* self, void*, void* bufferSlot, long* length)
{
    FableDiskHost* disk = reinterpret_cast<FableDiskHost*>(self);
    long request = length ? *length : 0;
    if (!disk->file || !bufferSlot || request <= 0)
    {
        if (length) *length = 0;
        return;
    }

    char* destination = *reinterpret_cast<char**>(bufferSlot);
    size_t got = fread(destination, 1, (size_t)request, disk->file);
    disk->position += (long)got;
    if (length) *length = (long)got;
}

extern "C" void FABLE_FASTCALL FableHostDiskRead(
    void* self, void*, void* destination, long length)
{
    FableDiskHost* disk = reinterpret_cast<FableDiskHost*>(self);
    if (!disk->file || !destination || length <= 0) return;
    size_t got = fread(destination, 1, (size_t)length, disk->file);
    disk->position += (long)got;
}

extern "C" void FABLE_FASTCALL FableHostReadOverflow_009A8840(
    void* self, void*, unsigned char* destination, long length)
{
    unsigned char* stream = reinterpret_cast<unsigned char*>(self);
    long remaining = length;
    unsigned char* out = destination;
    while (remaining > 0)
    {
        unsigned long available =
            *reinterpret_cast<unsigned long*>(stream + 0x14);
        if (available == 0)
        {
            void* source = *reinterpret_cast<void**>(stream + 0x18);
            void* buffer = *reinterpret_cast<void**>(stream + 0x1C);
            long request = (long)*reinterpret_cast<unsigned long*>(
                stream + 0x20);
            if (!source || !buffer || request <= 0)
                break;
            FableDiskHost* disk = reinterpret_cast<FableDiskHost*>(source);
            unsigned long desired =
                *reinterpret_cast<unsigned long*>(stream + 0x04);
            if (disk->file && (unsigned long)disk->position != desired)
            {
                if (fseek(disk->file, (long)desired, SEEK_SET) != 0)
                    break;
                disk->position = (long)desired;
            }
            void* bufferSlot = buffer;
            FableHostDiskReadNext(
                source, 0, &bufferSlot, &request);
            if (request <= 0)
                break;
            *reinterpret_cast<void**>(stream + 0x0C) = buffer;
            *reinterpret_cast<unsigned long*>(stream + 0x14) =
                (unsigned long)request;
            available = (unsigned long)request;
        }

        unsigned long take = available < (unsigned long)remaining
            ? available : (unsigned long)remaining;
        std::memcpy(
            out,
            *reinterpret_cast<void**>(stream + 0x0C),
            take);
        *reinterpret_cast<void**>(stream + 0x0C) =
            reinterpret_cast<unsigned char*>(
                *reinterpret_cast<void**>(stream + 0x0C)) + take;
        *reinterpret_cast<unsigned long*>(stream + 0x14) =
            available - take;
        *reinterpret_cast<unsigned long*>(stream + 0x04) += take;
        out += take;
        remaining -= (long)take;
    }
}

extern "C" long FABLE_FASTCALL FableHostBufferedGetPosition(void* self)
{
    return *reinterpret_cast<unsigned long*>(
        reinterpret_cast<unsigned char*>(self) + 4);
}

extern "C" void FABLE_FASTCALL FableHostBufferedReadNext(
    void* self, void*, void* bufferSlot, long* length)
{
    void* source = *reinterpret_cast<void**>(
        reinterpret_cast<unsigned char*>(self) + 0x18);
    unsigned char* bytes = reinterpret_cast<unsigned char*>(self);
    long request = (long)*reinterpret_cast<unsigned long*>(bytes + 0x20);
    if (request <= 0)
        request = 0x400000;
    FableHostDiskReadNext(source, 0, bufferSlot, &request);
    if (length)
        *length = request;
}

extern "C" void FABLE_FASTCALL FableHostBufferedRead(
    void* self, void*, void* destination, long length)
{
    void* source = *reinterpret_cast<void**>(
        reinterpret_cast<unsigned char*>(self) + 0x18);
    FableHostDiskRead(source, 0, destination, length);
}

extern "C" void FABLE_FASTCALL
CThreadedFileBaseConstruct_0099A2F0(void* self)
{
    std::memset(self, 0, 0x20);
}

extern "C" void FABLE_FASTCALL
CThreadedFileMemberConstruct_0099AED0(void* self)
{
    *reinterpret_cast<void**>(self) = 0;
}

extern "C" void* __cdecl
CFileDataInputStream_AllocateBuffer_00994700(unsigned long size)
{
    return std::malloc(size);
}

extern "C" void FABLE_FASTCALL
CFileDataInputStream_Close_00994780(void*, void*)
{
}

extern "C" void FABLE_FASTCALL
CFileDataInputStream_ConstructBase_00994700(void* self, void*)
{
    CFileDataInputStream_VTable_00994700[7] = (void*)FableHostBufferedGetPosition;
    CFileDataInputStream_VTable_00994700[8] = (void*)FableHostBufferedReadNext;
    CFileDataInputStream_VTable_00994700[9] = (void*)FableHostBufferedCanRead;
    CFileDataInputStream_VTable_00994700[10] = (void*)FableHostBufferedRead;
    std::memset(self, 0, 0x18);
}

extern "C" void FABLE_FASTCALL
CFileDataInputStream_DestroyBase_00994780(void*, void*)
{
}

extern "C" void FABLE_FASTCALL
CFileDataInputStream_SetPosition_00994700(void* self, void*, unsigned long pos)
{
    *reinterpret_cast<unsigned long*>(reinterpret_cast<unsigned char*>(self) + 4) = pos;
}

extern "C" void FABLE_FASTCALL
FableGetCurrentPath_00997F80()
{
}

extern "C" void FABLE_FASTCALL
ExpandPath(void*, void*)
{
}

extern "C" void FABLE_FASTCALL
FableThreadedCharConstruct_0098E1E0(void* self)
{
    *reinterpret_cast<void**>(self) = 0;
}

extern "C" void FABLE_FASTCALL
FableThreadedCharDestroy_0098E1E0(void*)
{
}

extern "C" void* FABLE_FASTCALL
FableThreadedCharToUpper_0098E1E0(void* string)
{
    return string;
}

extern "C" void FABLE_FASTCALL
FableThreadedConvertFullPath_0098E1E0(void* result, void* pathname)
{
    *reinterpret_cast<void**>(result) =
        pathname ? *reinterpret_cast<void**>(pathname) : 0;
}

extern "C" void FABLE_FASTCALL
FableThreadedFileObjectDelete_009A9040(void* object)
{
    FILE* file = *reinterpret_cast<FILE**>(
        reinterpret_cast<unsigned char*>(object) + 8);
    if (file && file != (FILE*)-1)
        fclose(file);
    std::free(object);
}

// The retail reset body stores this callback with a two-argument fastcall
// type (object plus the unused hidden register slot).  The host implementation
// only needs the object and already has the equivalent @4 entry point.
#pragma comment(linker, "/alternatename:@FableThreadedFileObjectDelete_009A9040@8=@FableThreadedFileObjectDelete_009A9040@4")

extern "C" void FABLE_FASTCALL
FableThreadedSplitPath_0098E1E0(void* drive, void* directory, void*, void*, void*)
{
    *reinterpret_cast<void**>(drive) = 0;
    *reinterpret_cast<void**>(directory) = 0;
}

extern "C" const wchar_t* FABLE_FASTCALL
FableThreadedWideConversion_0098E1E0(const void* value)
{
    return FableHostPathText(value);
}

extern "C" void FABLE_FASTCALL
FableThreadedWideAssign_0098E1E0(void* destination, const void* source)
{
    *reinterpret_cast<void**>(destination) =
        source ? *reinterpret_cast<void* const*>(source) : 0;
}

extern "C" void* __cdecl
FableThreadedFileControlAllocate_009A9C80(unsigned long size)
{
    return std::malloc(size);
}

extern "C" void __cdecl
FableThreadedFileControlDelete_009A9C80(void* control)
{
    std::free(control);
}

struct FableMapNodeHost
{
    unsigned char header[0x10];
    void* keyData;
    unsigned long values[5];
    FableMapNodeHost* next;
};

struct FableMapHost
{
    FableMapNodeHost* head;
    unsigned long size;
    unsigned long allocator;
};

static void FableHostCharAddRef(void* storage)
{
    if (storage)
        ++reinterpret_cast<FableCharDataHost*>(storage)->owners;
}

static void FableHostCharRelease(void* stringObject)
{
    void** slot = reinterpret_cast<void**>(stringObject);
    FableCharDataHost* data = slot
        ? reinterpret_cast<FableCharDataHost*>(*slot) : 0;
    if (!data)
        return;
    if (--data->owners == 0)
    {
        std::free(data->text);
        std::free(data);
    }
    *slot = 0;
}

static FableMapNodeHost* FableMapSentinel(FableMapHost* map)
{
    if (!map->head)
    {
        map->head = (FableMapNodeHost*)std::calloc(1, sizeof(FableMapNodeHost));
        map->size = 0;
    }
    else if (map->size == 0)
    {
        // Retail's constructor initializes the four tree-header words but
        // leaves the host-only linear walk link beyond that header dirty.
        reinterpret_cast<FableMapNodeHost*>(map->head)->next = 0;
    }
    return map->head;
}

extern "C" void FABLE_FASTCALL
FableContainedBankMapStringDestroy_009AC530(void* string, void*)
{
    FableHostCharRelease(string);
}

extern "C" void FABLE_FASTCALL
FableContainedBankMapInsert_009AC530(
    void* mapValue, void*, void** result, void*, const void* value)
{
    FableMapHost* map = reinterpret_cast<FableMapHost*>(mapValue);
    FableMapNodeHost* sentinel = FableMapSentinel(map);
    FableMapNodeHost* node = (FableMapNodeHost*)std::calloc(
        1, sizeof(FableMapNodeHost));
    if (!node)
    {
        if (result) *result = 0;
        return;
    }
    std::memcpy(&node->keyData, value, sizeof(void*) + 5 * sizeof(unsigned long));
    FableHostCharAddRef(node->keyData);
    node->next = sentinel->next;
    sentinel->next = node;
    ++map->size;
    if (result) *result = node;
}

extern "C" void FABLE_FASTCALL
FableContainedBankMapStringCopy_009AC530(void* destination, void*, const void* source)
{
    void* storage = *reinterpret_cast<void* const*>(source);
    *reinterpret_cast<void**>(destination) = storage;
    FableHostCharAddRef(storage);
}

extern "C" bool FABLE_FASTCALL
FableContainedBankMapStringLess_009AC530(const void*, void*, const void*)
{
    return false;
}

extern "C" void* FABLE_FASTCALL
FableContainedBankMapLowerBound_009AC530(void* mapValue, void*, const void*)
{
    return FableMapSentinel(reinterpret_cast<FableMapHost*>(mapValue));
}
