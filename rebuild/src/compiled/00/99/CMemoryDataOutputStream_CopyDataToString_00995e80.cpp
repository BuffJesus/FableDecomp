// lib_data_stream.cpp, retail 0x00995E80 (Ego_r
// ?CopyDataToString@CMemoryDataOutputStream@@QBEXAAVCCharString@@@Z): the
// written bytes, as they stand, into a string.
#include "fable_data_output_stream.h"
#include "fable_carray.h"
#include "fable_string.h"

void CMemoryDataOutputStream::CopyDataToString(CCharString& string) const
{
    string.SetAsSubString(Data.GetAsCArray(), Data.size());
}
