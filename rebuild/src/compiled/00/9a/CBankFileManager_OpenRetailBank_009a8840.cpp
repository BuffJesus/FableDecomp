#include "fable_data_input_stream.h"
#include "fable_disk_file.h"
#include "fable_retail_banks.h"
#include "fable_stlport_vector.h"

// Retail expands this empty novtable constructor and calls its base directly.
inline CAFile::CAFile() {}

namespace
{
    // Retail never hands the stream a null buffer: an empty vector yields
    // its own address instead of its (null) first element.
    inline fable_u8* BufferData(_STL::vector<fable_u8>& buffer)
    {
        return buffer.size() ?
            &buffer[0] :
            reinterpret_cast<fable_u8*>(&buffer);
    }
}

// CBankFileManager::OpenRetailBank(CWideString const&, bool) @ 0x009A8840.
//
// Opens a BIGB bank, registers it as a retail bank, reads its header (the
// contained-bank table: name plus five dwords each) and then reopens the
// file through a CThreadedFile for streaming.
void CBankFileManager::OpenRetailBank(
    const CWideString& pathname,
    bool startupPriority)
{
    if (!CAFile::PathExists(pathname))
        return;

    CCountedPointer<CDiskFileWin32> file(new CDiskFileWin32);
    file->Open(pathname, FileAccessType(1), 2);
    CFileDataInputStream stream(file.Data, 0x4000);

    char id[4];
    stream >> id;
    if (memcmp(id, "BIGB", 4) == 0)
    {
        if (stream.ReadValue<fable_u32>() != retailBankVersion_)
            return;

        CCountedPointer<NBankFileManager::CRetailBank> bank(
            new NBankFileManager::CRetailBank);
        RetailBanks.push_back(bank);
        bank->filePath_ = pathname.ToCharString();
        bank->diskFile_ = file;

        fable_u32 headerOffset = stream.ReadValue<fable_u32>();
        fable_u32 headerSize = stream.ReadValue<fable_u32>();
        if (headerSize > 0)
        {
            _STL::vector<fable_u8> header(headerSize);
            stream.SetPosition(headerOffset);
            stream.Read(BufferData(header), headerSize);

            CMemoryDataInputStream headerStream(
                BufferData(header),
                headerSize);
            fable_u32 containedBankCount =
                headerStream.ReadValue<fable_u32>();
            for (fable_u32 i = 0; i < containedBankCount; ++i)
            {
                CCharString name = headerStream.ReadNullTerminatedString();
                fable_u32 dataVersion = headerStream.ReadValue<fable_u32>();
                fable_u32 entryCount = headerStream.ReadValue<fable_u32>();
                fable_u32 infoOffset = headerStream.ReadValue<fable_u32>();
                fable_u32 infoSize = headerStream.ReadValue<fable_u32>();
                fable_u32 alignment = headerStream.ReadValue<fable_u32>();

                NBankFileManager::CContainedBank& contained =
                    bank->containedBanks_[name];
                contained.dataVersion_ = dataVersion;
                contained.entryCount_ = entryCount;
                contained.infoOffset_ = infoOffset;
                contained.infoSize_ = infoSize;
                contained.alignment_ = alignment;
            }
        }

        bank->threadedFile_.Reset(new CThreadedFile);
        bank->useDma_ = false;
        bank->threadedFile_->Open(pathname, false);
    }
}
