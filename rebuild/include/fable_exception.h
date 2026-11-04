#pragma once
// Ego_r.pdb layouts and retail throw metadata at 013692F8: both classes are
// one byte, with CGenericException publicly deriving from CExceptionBase.
class CExceptionBase {};
class CGenericException : public CExceptionBase
{
public:
    CGenericException() {}
    // The release error macro passes its message here and drops it
    // (CBankFileManager::GetDevBankFilePath 0x009A7CA0 builds the text first,
    // and the temporary is still live at the throw).
    explicit CGenericException(const char*) {}
};
