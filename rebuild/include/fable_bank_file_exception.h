#pragma once
#include "fable_exception.h"
#include "fable_string.h"

// Ego_r donor class/enum names and layout; retail throw metadata 01369470.
class CBankFileException : public CExceptionBase
{
public:
    enum EBankFileExceptionType
    {
        BANK_IS_CORRUPT = 0,
        BANK_IS_INCORRECT_VERSION = 1,
        ENTRY_NOT_FOUND = 2,
        ENTRY_IS_CORRUPT = 3
    };
    // Retail 009D1E70, by-value message; its storage is released on return.
    CBankFileException(CCharString errortext, EBankFileExceptionType type);
    explicit CBankFileException(EBankFileExceptionType type) : Type(type) {}
    EBankFileExceptionType Type;
};
FABLE_STATIC_ASSERT(sizeof(CBankFileException) == 4);
