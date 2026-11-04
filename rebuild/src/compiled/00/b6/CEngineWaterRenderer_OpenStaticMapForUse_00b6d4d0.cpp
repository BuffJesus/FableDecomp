#include "engine/CEngineWaterRenderer.h"

void CEngineWaterRenderer::OpenStaticMapForUse(CStaticMapBankFile* bank,CCharString prefix)
{
    StaticMapBankFile=bank;
    StaticMapSeaEntryPrefix=prefix;
}
