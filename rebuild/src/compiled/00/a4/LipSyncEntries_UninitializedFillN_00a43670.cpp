// 0x00A43670: STLport's uninitialized_fill_n for CLipSyncDataBank's entries (declared in
// fable_stlport_uninitialized.h), out of line in retail and called from vector::_M_fill_insert 0x00A43CD0. A
// unit of its own, as for <CCharString> (0x00433504): with the body in the vector's unit VC7.1 inlines it there.
#include <new>

#include "fable_lipsync_data_bank.h"

template <class T>
T* FABLE_FASTCALL _STL::uninitialized_fill_n(T* first, size_t count, const T& value)
{
    T* cur = first;
    for (; count > 0; --count, ++cur)
        _Construct(cur, value);
    return cur;
}
template CLipSyncDataBankEntry* FABLE_FASTCALL _STL::uninitialized_fill_n<CLipSyncDataBankEntry>(
    CLipSyncDataBankEntry*, size_t, const CLipSyncDataBankEntry&);
