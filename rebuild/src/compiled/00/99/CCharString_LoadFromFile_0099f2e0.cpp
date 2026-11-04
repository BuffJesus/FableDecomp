#include "fable_base_class.h"

// PDB shapes (Ego_r.pdb). fable_string.h keeps CCharStringData as a flat
// aggregate for fixtures that brace-initialise it, so this file carries the
// real CBasicString-backed layout whose inline constructors retail expands here.
class CWideString;
class CASuspendableProcess;

enum FileAccessType
{
};

class CAFile : public CBaseClassNonCopyable
{
public:
    virtual void Open(
        const CWideString& pathname,
        FileAccessType accessType,
        unsigned long flags) = 0;
    virtual void Close() = 0;
    virtual void Read(
        void* data,
        long length,
        CASuspendableProcess* process) = 0;
    virtual void Write(
        const void* data,
        long length,
        CASuspendableProcess* process) = 0;
    virtual void SetPosition(long position) = 0;
    virtual void MovePosition(long offset) = 0;
    virtual long GetPosition() const = 0;
    virtual void SetLength(long length) = 0;
    virtual long GetLength() const = 0;
};

#pragma pack(push, 1)
template<typename T>
class CBasicString
{
public:
    CBasicString(unsigned long length, T fill)
        : PData(0),
          StringLength(0),
          DataLength(0),
          UseFastExtend(true)
    {
        SetString(length, fill);
    }

    T* PData;
    unsigned long StringLength;
    unsigned long DataLength : 31;
    bool UseFastExtend : 1;

protected:
    void SetString(unsigned long length, T fill);
};

struct CCharStringData
{
    CCharStringData(unsigned long length, char fill)
        : Data(length, fill),
          NoRefs(1)
    {
    }

    CBasicString<char> Data;
    long NoRefs;
};
#pragma pack(pop)

class CCharString
{
public:
    void LoadFromFile(
        CAFile& file,
        CASuspendableProcess* process,
        long length);

private:
    void UnassignString();

    CCharStringData* PStringData;
};

// CCharString::LoadFromFile(CAFile&, CASuspendableProcess*, long) @ 0x0099F2E0.
// A length of -1 reads from the current position to the end of the file.
void CCharString::LoadFromFile(
    CAFile& file,
    CASuspendableProcess* process,
    long length)
{
    if (PStringData)
        UnassignString();

    if (length == -1)
        length = file.GetLength() - file.GetPosition();

    PStringData = new CCharStringData(length, ' ');
    if (PStringData && length > 0)
        file.Read(PStringData->Data.PData, length, process);
}
