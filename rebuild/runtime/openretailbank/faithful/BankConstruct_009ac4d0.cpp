#include <new>
#include <stdlib.h>

typedef unsigned char fable_u8;
typedef unsigned long fable_u32;

// Minimal base object with its own vtable-setting constructor; retail calls
// this base subobject constructor first (ecx==this), then the derived
// constructor overwrites the vtable pointer with the derived vtable.
class CBase
{
public:
    __declspec(noinline) CBase();

    fable_u32 vtable_;
};

extern "C" fable_u8 g_CBaseVTable_009AC4D0;

__declspec(noinline) CBase::CBase()
{
    vtable_ = reinterpret_cast<fable_u32>(&g_CBaseVTable_009AC4D0);
}

// Minimal narrow-string value type matching retail CCharString: a single
// storage pointer, default-constructed to null.
class CCharString
{
public:
    __declspec(noinline) CCharString();

private:
    void* storage_;
};

__declspec(noinline) CCharString::CCharString()
{
    storage_ = 0;
}

extern "C" fable_u8 g_FableRetailBankVTable_0129B010;

namespace NBankFileManager
{
    class CRetailBank : public CBase
    {
    public:
        CRetailBank();

        void* diskFile_;
        void* diskFileControl_;
        void* threadedFile_;
        void* threadedFileControl_;
        bool useDma_;
        fable_u8 padding15_[3];
        void* containedBanksHead_;
        fable_u32 containedBankCount_;
        fable_u32 containedBanksAllocator_;
        fable_u8 filePathStorageRaw_[sizeof(void*)];
    };
}

// NBankFileManager::CRetailBank::CRetailBank() @ 0x009AC4D0.
// filePathStorage_ (a CCharString) is placement-constructed at the end of
// the body, after the contained-bank list head is allocated and
// initialised, matching retail's call ordering exactly. It is stored as raw
// bytes in the class layout (filePathStorageRaw_) rather than as a genuine
// CCharString member so that its default constructor is not implicitly
// invoked by the member-initialisation sequence (which runs before the
// body), reproducing retail's actual call order for this subobject.
//
// containedBanksHead_ is re-read from the member (rather than cached in a
// local) between each store to the freshly allocated node, matching
// retail's own repeated reloads of NBankFileManager::CRetailBank
// containedBanksHead_.
NBankFileManager::CRetailBank::CRetailBank()
{
    vtable_ = reinterpret_cast<fable_u32>(&g_FableRetailBankVTable_0129B010);
    diskFile_ = 0;
    diskFileControl_ = 0;
    threadedFile_ = 0;
    threadedFileControl_ = 0;
    useDma_ = false;

    containedBanksHead_ = 0;
    containedBanksHead_ =
        reinterpret_cast<fable_u8*>(malloc(0x28));
    containedBankCount_ = 0;

    *reinterpret_cast<fable_u8*>(containedBanksHead_) = 0;
    *reinterpret_cast<fable_u32*>(
        reinterpret_cast<fable_u8*>(containedBanksHead_) + 4) = 0;
    *reinterpret_cast<void**>(
        reinterpret_cast<fable_u8*>(containedBanksHead_) + 8) =
        containedBanksHead_;
    *reinterpret_cast<void**>(
        reinterpret_cast<fable_u8*>(containedBanksHead_) + 0x0C) =
        containedBanksHead_;

    reinterpret_cast<CCharString*>(filePathStorageRaw_)->CCharString::CCharString();
}
