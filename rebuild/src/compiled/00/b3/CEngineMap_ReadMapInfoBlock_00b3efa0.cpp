#include "engine/CEngineMap.h"
#include "fable_data_input_stream.h"

void FABLE_FASTCALL CEngineMap::ReadMapInfoBlock(CStaticMapInfoBlock& info,CDataInputStream& stream)
{
    info.VersionID=stream.ReadValue<long>();
    info.BankFileIndex=stream.ReadValue<long>();
    info.EdgeHeightFileSize=stream.ReadValue<long>();
    info.MapWidth=stream.ReadValue<long>();
    info.MapHeight=stream.ReadValue<long>();
    info.WorldX=stream.ReadValue<long>();
    info.WorldY=stream.ReadValue<long>();
    info.EdgeHeightFilePtr=stream.ReadValue<long>();
    info.LandscapeMapPtr=stream.ReadValue<long>();
    info.LocalDetailMapPtr=stream.ReadValue<long>();
    info.Quality=stream.ReadValue<long>();
    info.ShorePointArraySize=stream.ReadValue<long>();
    info.ShorePointArrayStart=stream.ReadValue<long>();
    info.LevelChecksum=stream.ReadValue<long>();
    info.ChecksumBlockFilePtr=stream.ReadValue<long>();
    info.ChecksumBlockFileSize=stream.ReadValue<long>();
    stream.Read(&info.CameraMapBounds,sizeof(info.CameraMapBounds));
    info.HeaderEndPtr=stream.ReadValue<long>();
}
