#include "fable_def_class_base.h"
#include "fable_data_output_stream.h"
// The three flag bits, one byte each.
void CDefClassBase::InternalSaveBinary(CDataOutputStream& stream)
{
    stream.WriteUBYTE(Setup);
    stream.WriteUBYTE(Template);
    stream.WriteUBYTE(DefaultValsApplied);
}
