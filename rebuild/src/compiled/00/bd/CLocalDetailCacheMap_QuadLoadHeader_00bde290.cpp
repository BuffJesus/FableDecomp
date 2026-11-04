#include "engine/CLocalDetailCacheMap.h"
#include "fable_data_input_stream.h"

void CLocalDetailCacheMap::CQuadTreeElement::LoadHeader(CDataInputStream& stream)
{
    CFileHeader header;
    stream.Read(&header,sizeof(header));
    MapX=header.MapX;
    MapY=header.MapY;
    Width=header.Width;
    Height=header.Height;
    FileBlockPos=header.FileBlockPos;
    FileBlockSize=header.FileBlockSize;
    OffsetIntoFileBlock=header.OffsetIntoFileBlock;
    MaxFadeDistance=header.MaxFadeDistance;
    PrimitiveTypeMask=header.PrimitiveTypeMask;
    BoundingSphere=header.BoundingSphere;
}
