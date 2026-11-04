#include "fable_bank_file_exception.h"

CBankFileException::CBankFileException(CCharString errortext,
                                      EBankFileExceptionType type)
{
    static_cast<const char*>(errortext);
    Type = type;
}
