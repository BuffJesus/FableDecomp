#include <new>
#include "fable_game_text_bank.h"

// Retail's non-POD copy path assigns each aggregate field; the current
// shared vector erase helper only models the POD memmove path.
template<> __forceinline CBankFile::CRuntimeData*
_STL::vector<CBankFile::CRuntimeData>::erase(CBankFile::CRuntimeData* first, CBankFile::CRuntimeData* last)
{
    CBankFile::CRuntimeData* out = first;
    for (int count = _M_finish - last; count > 0; --count) {
        *out = *last;
        ++last;
        ++out;
    }
    _M_finish = out;
    return first;
}
template<> inline _STL::pair<unsigned long, unsigned long>*
_STL::vector<_STL::pair<unsigned long, unsigned long> >::erase(
    _STL::pair<unsigned long, unsigned long>* first, _STL::pair<unsigned long, unsigned long>* last)
{
    _STL::pair<unsigned long, unsigned long>* out = first;
    for (int count = _M_finish - last; count > 0; --count) {
        *out = *last;
        ++last;
        ++out;
    }
    _M_finish = out;
    return first;
}

#include "fable_small_vector.h"
// Recovered prefix used by 009CD850; trailing fields still require evidence.
class CBankFileEntryUpdateData {
public:
    unsigned long StateBlockCRC, InfoSize;
    char* InfoBuffer;
    CSmallVector<CCharString, 8> Filenames;
    ~CBankFileEntryUpdateData() {
        if (InfoBuffer) {
            delete[] InfoBuffer;
            InfoBuffer = 0;
        }
        InfoSize = 0;
        Filenames.Clear();
    }
};

template<class T> void ReleaseArray(CArrayVector<T>& array)
{
    if (array.capacity() > 0) {
        array.clear();
        _STL::vector<T> empty;
        array.swap(empty);
    }
}
// Existing out-of-line helpers at 009D3FB0 and 00A629D0.
void __fastcall ReleaseBankSymbols(CArrayVector<CCharString>*);
void __fastcall ClearPackedBankArray(CPackedUIntArray*);

void __fastcall CloseBankFileImpl(CBankFile* self)
{
    for (CBankFileEntryUpdateData** p = self->UpdateData.begin(); p != self->UpdateData.end(); ++p)
        delete *p;
    ReleaseArray(self->UpdateData);
    ReleaseArray(self->RuntimeData);
    ReleaseBankSymbols(&self->Symbols);
    if (self->BankFile.Info) {
        --self->BankFile.Info->RefCount;
        if (self->BankFile.Info->RefCount == 0) {
            self->BankFile.Info->DeleteFunc(self->BankFile.Info->Data);
            delete self->BankFile.Info;
        }
    }
    self->BankFile.Info = 0;
    self->BankFile.Data = 0;
    self->SymbolIndices.clear();
    self->SymbolCRCIndices.clear();
    self->OldSymbolIndices.clear();
    self->FilenameIndices.clear();
    ClearPackedBankArray(&self->PackedDataOffset);
    ClearPackedBankArray(&self->PackedDataSize);
    ClearPackedBankArray(&self->PackedDataType);
    ClearPackedBankArray(&self->PackedValid);
    self->Size = 0;
}
