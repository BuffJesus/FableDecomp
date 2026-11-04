// lib_bank_file_manager.cpp, retail 0x009A7C60 / 0x009A7CA0 / 0x009A7E20 / 0x009A7ED0 (Ego_r
// CBankFileManager::LookupBankAlias / GetDevBankFilePath / GetDevHeaderFilePath /
// GetRetailHeaderFilePath): the bank's
// header path under the base path, after resolving any alias; empty when the
// bank or its path is unknown.
#include "fable_retail_banks.h"
#include "fable_exception.h"

// Retail 009A7C60.
CCharString CBankFileManager::LookupBankAlias(const CCharString& bank_handle) const
{
    return Aliases.Resolve(bank_handle);
}

// Retail 009A7CA0: an unknown bank throws; the message goes to the exception's
// (ignored) text constructor.
CWideString CBankFileManager::GetDevBankFilePath(const CCharString& bank_handle) const
{
    CCharString resolved = Aliases.Resolve(bank_handle);
    CBankInfoMap::CNode* node = Banks.Find(resolved);
    if (node != Banks.head_)
        return BasePath() + static_cast<CBankInfoMap::CEntryNode*>(node)->value_.second.DevBankFilePath;
    throw CGenericException(resolved + " bank not found!");
}

// Retail 009A7E20.
CWideString CBankFileManager::GetDevHeaderFilePath(const CCharString& bank_handle) const
{
    CCharString resolved = Aliases.Resolve(bank_handle);
    CBankInfoMap::CNode* node = Banks.Find(resolved);
    if (node != Banks.head_)
    {
        CWideString path = static_cast<CBankInfoMap::CEntryNode*>(node)->value_.second.DevHeaderFilePath;
        if (path.GetLength() != 0)
            return BasePath() + path;
    }
    return CWideString(L"");
}

// Retail 009A7ED0.
CWideString CBankFileManager::GetRetailHeaderFilePath(const CCharString& bank_handle) const
{
    CCharString resolved = Aliases.Resolve(bank_handle);
    CBankInfoMap::CNode* node = Banks.Find(resolved);
    if (node != Banks.head_)
    {
        CWideString path = static_cast<CBankInfoMap::CEntryNode*>(node)->value_.second.RetailHeaderFilePath;
        if (path.GetLength() != 0)
            return BasePath() + path;
    }
    return CWideString(L"");
}
