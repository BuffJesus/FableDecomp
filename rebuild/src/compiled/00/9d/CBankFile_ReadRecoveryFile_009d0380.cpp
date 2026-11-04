// CBankFile::ReadRecoveryFile @ 009D0380. Normal return paths match retail.
// The entry-list result is deliberately ignored; successful recovery header
// validation returns true after attempting to load entries.
#include "fable_game_text_bank.h"
#include "fable_disk_file.h"
#include "fable_data_input_stream.h"
inline CAFile::CAFile() {}
bool CBankFile::ReadRecoveryFile(unsigned long validate)
{
    CWideString filename = GetRecoveryFilePath();
    if (CAFile::PathExists(filename))
    {
        CDiskFileWin32 recoveryFile;
        recoveryFile.Open(filename, FILE_ACCESS_READ, 4);
        CFileDataInputStream stream(&recoveryFile, 0x4000);
        char header[4];
        char correct_header[] = "BREC";
        stream.Read(header, 4);
        if (memcmp(header, correct_header, 4) != 0) return false;
        if (stream.ReadValue<unsigned long>() != validate) return false;
        unsigned long max_index = stream.ReadValue<unsigned long>();
        unsigned long entry_count = stream.ReadValue<unsigned long>();
        ReadEntryList(stream, max_index, entry_count);
        return true;
    }
    return false;
}
