#include "fable_bank_entry_pointer_inline.h"
#include "engine/CEngineMap.h"
#include "engine/CStaticMapBankFile.h"
#include "engine/CEngineLandscapeMap.h"
#include "engine/CLocalDetailCacheMap.h"
#include "fable_data_input_stream.h"

void CEngineMap::OpenRetailStaticMap(CStaticMapBankFile& bank,CDataInputStream& stream,
                                   _STL::map<CCharString,long>& positions)
{
    _STL::map<CCharString,long>::iterator position=positions.find(StaticMapName);
    CloseStaticMap();
    stream.SetPosition(position->second);
    CStaticMapInfoBlock info;
    ReadMapInfoBlock(info,stream);
    CameraMapBounds=info.CameraMapBounds;
    AsyncFileEntry=bank.GetAsyncEntry(info.BankFileIndex);
    FileBlockManager.SetBankFileEntry(AsyncFileEntry);
    stream.SetPosition(info.LandscapeMapPtr);
    LandscapeMap->OpenStaticMap(&FileBlockManager,stream);
    if (LocalDetailCacheMap) {
        stream.SetPosition(info.LocalDetailMapPtr);
        LocalDetailCacheMap->OpenStaticMap(&FileBlockManager,stream);
    }
    stream.SetPosition(info.HeaderEndPtr);
    StaticMapOpenState=STATIC_MAP_OPEN_STATE_OPEN_FOR_USE;
}
