#include "fable_def_class_base.h"
#include "fable_data_input_stream.h"
void CDefClassBase::InternalLoadBinary(CDataInputStream& stream)
{
    Setup = stream.ReadValue<unsigned char>();
    Template = stream.ReadValue<unsigned char>();
    DefaultValsApplied = stream.ReadValue<unsigned char>();
}