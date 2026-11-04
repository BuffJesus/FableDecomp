#include "engine/CLocalDetailCacheMap.h"
#include "fable_data_input_stream.h"
void CLocalDetailCacheMap::OpenStaticMap(CEngineStaticMapFileBlockManager* manager,CDataInputStream& stream)
{
    StaticMapFileBlockManager=manager;
    CFileHeader header;
    stream.Read(&header,sizeof(header));
    stream.SetPosition(header.DataFilePos);
    QuadTreeElement.LoadHeader(stream);
    ObjectTypeCollectionPalette.Load(stream);
}
