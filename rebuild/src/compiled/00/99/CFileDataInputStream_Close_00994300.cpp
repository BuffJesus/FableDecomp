// CFileDataInputStream::Close  (retail 0x00994300)
// Faithful VC7.1 reconstruction.

extern "C" void __cdecl FreeStreamBuffer(void* p);

// Abstract base with true (default) thiscall member-function calling
// convention: this-in-ecx, remaining args pushed on the stack.
class IOwnedFile
{
public:
    virtual void   Slot00() = 0;
    virtual void   Slot04() = 0;
    virtual void   Slot08() = 0;
    virtual void   Slot0C() = 0;
    virtual void   Slot10() = 0;
    virtual void   ReleaseHandle(int id) = 0; // vtbl+0x14 (index 5)
    virtual void   Slot18() = 0;
    virtual void   Slot1C() = 0;
    virtual void   Slot20() = 0;
    virtual void   Slot24() = 0;
    virtual bool   ShouldRelease() = 0;       // vtbl+0x28 (index 10)
};

class CFileDataInputStream
{
public:
    virtual void Unused00() = 0;
    virtual void Unused04() = 0;
    virtual int  GetHandleId() = 0;          // vtbl+0x08 (index 2)

    void* field04;                    // +0x04
    void* field08;                    // +0x08
    void* field0C;                    // +0x0C
    void* field10;                    // +0x10
    void* field14;                    // +0x14
    IOwnedFile* ownedFile;             // +0x18
    void* buffer;                      // +0x1C
    int   bufferCount;                 // +0x20

    void Close();
};

void CFileDataInputStream::Close()
{
    if (buffer != 0)
    {
        FreeStreamBuffer(buffer);
        buffer = 0;
        bufferCount = 0;
    }

    if (ownedFile != 0)
    {
        if (ownedFile->ShouldRelease())
        {
            ownedFile->ReleaseHandle(GetHandleId());
        }
    }

    ownedFile = 0;
    field04 = 0;
    field08 = 0;
    field10 = 0;
    field14 = 0;
    field0C = 0;
}
