#include "engine/CWaterStaticMapInfoBlock.h"
#include "fable_data_input_stream.h"

bool CWaterStaticMapInfoBlock::Load(CMemoryDataInputStream& stream,long size)
{
    if (size != sizeof(*this))
        return false;
    stream.Read(this,sizeof(*this));
    return true;
}
