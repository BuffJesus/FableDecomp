// CParentDefClassBase::InternalSaveBinary(CDataOutputStream&) @ 0x009B0200
// (FableWin ?InternalSaveBinary@CParentDefClassBase@@UAEXAAVCDataOutputStream@@@Z).
#include "fable_def_class_base.h"
#include "fable_data_output_stream.h"
// The base flags (CDefClassBase::InternalSaveBinary, expanded in this unit),
// then the sorted sub-def table: a 16-bit count and each key and index pair.
void CParentDefClassBase::InternalSaveBinary(CDataOutputStream& stream)
{
    stream.WriteUBYTE(Setup);
    stream.WriteUBYTE(Template);
    stream.WriteUBYTE(DefaultValsApplied);
    SubDefInfoMap.Update();
    unsigned long count = SubDefInfoMap.size();
    stream.WriteUWORD((unsigned short)count);
    for (unsigned long index = 0; index < count; ++index)
    {
        stream.WriteULONG(SubDefInfoMap[index].first);
        const CSubDefInfo& info = SubDefInfoMap[index].second;
        stream.WriteSLONG(info.DefIndex);
        stream.WriteSLONG(info.OriginalParentDefIndex);
    }
}
