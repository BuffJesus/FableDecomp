// CWADFile (retail WADFile unit 0x00A39BA0..0x00A39E10): the CAFile interface over one entry of a WAD bank.
// Reads come from the bank entry at a private file position; writes and length changes are ignored.
#include "fable_wad_file.h"
#include "fable_file_information.h"
#include "fable_bank_entry_update.h"

inline CAFile::CAFile() {}

// 0x00A39BA0
void CWADFile::Close()
{
    Index = 0;
    FilePos = 0;
}

// 0x00A39BB0
void CWADFile::Read(void* data, long size, CASuspendableProcess* process)
{
    Bank.ReadEntryData(Index, FilePos, data, size);
    FilePos += size;
}

// 0x00A39BE0
void CWADFile::Write(const void* data, long size, CASuspendableProcess* process)
{
}

// 0x00A39BF0
void CWADFile::SetPosition(long position)
{
    FilePos = position;
}

// 0x00A39C00
void CWADFile::MovePosition(long offset)
{
    FilePos += offset;
}

// 0x00A39C10
long CWADFile::GetPosition() const
{
    return FilePos;
}

// 0x00A39C20
long CWADFile::GetLength() const
{
    return Bank.GetEntryDataSize(Index);
}

// 0x00A39C30
bool CWADFile::IsIOFinished() const
{
    return true;
}

// 0x00A39C40
CWideString CWADFile::GetPathname() const
{
    return Bank.GetEntrySymbol(Index).ToWideString();
}

// 0x00A39C70
long CWADFile::GetAlignment() const
{
    return Bank.GetAlignment();
}

// 0x00A39C80
CAFile* CWADFile::Clone() const
{
    return new CWADFile(*this);
}

// 0x00A39CC0: the entry is looked up by the CRC of the narrow pathname.
void CWADFile::InternalOpen(const CWideString& pathname, FileAccessType accessType, unsigned long flags)
{
    Index = Bank.FindIndexBySymbolCRC(pathname.ToCharString().GetCRC());
    FilePos = 0;
}

// 0x00A39D20. Ego_r reads the entry through the inline CBankFile::PeekEntryUpdateData(Index) (not declared in the
// shared bank header yet); retail expands it to UpdateData[Index].
CFileInformation CWADFile::GetFileInformation()
{
    CFileInformation info;
    Bank.UpdateData[Index]->ReadInfo(0, &info, sizeof(info));
    return info;
}

// 0x00A39D80
CWADFile::CWADFile(CWADBank& bank, const CWideString& pathname, FileAccessType accessType, unsigned long flags)
    : Bank(bank), Index(0), FilePos(0)
{
    InternalOpen(pathname, accessType, flags);
}

// 0x00A39DC0
void CWADFile::Open(const CWideString& pathname, FileAccessType accessType, unsigned long flags)
{
    InternalOpen(pathname, accessType, flags);
}

// 0x00A39DD0
void CWADFile::SetLength(long length)
{
}

// 0x00A39DE0
bool CWADFile::IsOpen() const
{
    return Index != 0;
}
