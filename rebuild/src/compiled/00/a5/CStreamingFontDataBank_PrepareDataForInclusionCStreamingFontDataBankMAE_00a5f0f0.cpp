// Byte-exact reconstruction of
// CStreamingFontDataBank::PrepareDataForInclusion (0x00a5f0f0)
//
// Retail body: push esi; load the by-value CCountedPointer<CBankStateBlock>
// control block from [esp+0x18]; if non-null, decrement its refcount, and on
// zero call the block's release fn ptr (ecx=block+8 payload, call [block+4])
// then free the block via a cdecl helper (push;call;add esp,4). ret 0x1c.
//
// Signature reproduced from the decorated name so VC7.1 mangles identically:
//   protected virtual void __fastcall PrepareDataForInclusion(
//       unsigned long, const CArray<CCharString>&, unsigned long,
//       CCountedPointer<CBankStateBlock>, CDataOutputStream&, CDataOutputStream&)

class CCharString {};

template <class T> class CArray { public: T* p; long n; long cap; };

// CCountedPointer's control block. Destruction decrements the shared count
// and, at zero, invokes the virtual release then frees the block.
struct RefBlock {
    long count;                                  // +0x00
    void (__fastcall *release)(void* payload);   // +0x04
    void* payload;                               // +0x08
};

extern "C" void __cdecl FreeRefBlock(RefBlock* b);

template <class T>
class CCountedPointer {
public:
    T* obj;           // +0x00: the pointed-to object (raw)
    RefBlock* ctrl;   // +0x04: shared control block
    ~CCountedPointer() {
        RefBlock* b = ctrl;
        if (b) {
            if (--b->count == 0) {
                b->release(b->payload);
                FreeRefBlock(b);
            }
        }
    }
};

class CBankStateBlock {};
class CDataOutputStream {};

class CStreamingFontDataBank {
protected:
    virtual void PrepareDataForInclusion(
        unsigned long a1,
        const CArray<CCharString>& a2,
        unsigned long a3,
        CCountedPointer<CBankStateBlock> a4,
        CDataOutputStream& a5,
        CDataOutputStream& a6);
};

void CStreamingFontDataBank::PrepareDataForInclusion(
        unsigned long a1,
        const CArray<CCharString>& a2,
        unsigned long a3,
        CCountedPointer<CBankStateBlock> a4,
        CDataOutputStream& a5,
        CDataOutputStream& a6)
{
    // Whole body is the destruction of the by-value counted pointer a4.
    (void)a1; (void)a2; (void)a3; (void)a5; (void)a6;
    // a4 destructor runs at end of scope.
}