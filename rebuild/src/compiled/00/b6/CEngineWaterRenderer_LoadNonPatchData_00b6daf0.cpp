#include "engine/CEngineWaterRenderer.h"
#include "engine/CWaterStaticMapInfoBlock.h"
#include "fable_data_input_stream.h"

void CEngineWaterRenderer::LoadNonPatchData(CMemoryDataInputStream& stream,
                                         CMemoryDataInputStream& infoStream)
{
    CWaterStaticMapInfoBlock info;
    info.Load(infoStream,infoStream.ReadValue<long>());
}
