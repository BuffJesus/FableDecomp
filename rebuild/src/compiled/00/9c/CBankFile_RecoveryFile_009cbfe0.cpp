// lib_bank_file.cpp, retail 0x009CBFE0 / 0x009CC160 (Ego_r names and locals):
// the recovery file sits beside the bank as "<bank name>.rec".
#include "fable_game_text_bank.h"
#include "fable_disk_file.h"
#include "fable_filesystem.h"

extern "C" __declspec(dllimport) int __stdcall DeleteFileW(const wchar_t*);

// Retail 009CBFE0.
CWideString CBankFile::GetRecoveryFilePath()
{
    CWideString path_drive;
    CWideString path_dir;
    CWideString path_filename;
    CAFile::SplitPath(BankFile->GetPathname(), &path_drive, &path_dir, &path_filename, 0);
    CWideString recovery_path = path_drive + path_dir + path_filename + L".rec";
    return recovery_path;
}

// Retail 009CC160.
void CBankFile::DeleteRecoveryFile()
{
    CWideString filename = GetRecoveryFilePath();
    if (CAFile::PathExists(filename))
        DeleteFileW(filename);
}
