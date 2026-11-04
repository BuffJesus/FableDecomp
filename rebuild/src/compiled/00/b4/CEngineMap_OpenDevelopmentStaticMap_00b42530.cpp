#include "fable_bank_entry_pointer_outofline.h"
#include "engine/CEngineMap.h"
#include "engine/CStaticMapBankFile.h"
#include "engine/CEngineLandscapeMap.h"
#include "engine/CLocalDetailCacheMap.h"
#include "fable_data_input_stream.h"
#include "fable_bank_entry_update.h"
#include <new>
#include "fable_stlport_vector_impl.h"

void CEngineMap::OpenDevelopmentStaticMap(CStaticMapBankFile& bank,STATIC_MAP_OPEN_STATE state)
{
    if(StaticMapOpenState==state) return;
    CloseStaticMap();
    unsigned long index=bank.FindIndexBySymbol(StaticMapName);
    if(!index) return;
    CArrayVector<char> data;
    CMemoryDataInputStream stream;
    CBankFileEntryUpdateData* entry=bank.UpdateData[index];
    unsigned long length=entry->InfoSize;
    data.resize(length);
    entry->ReadInfo(0,data.GetAsCArray(),length);
    stream.OpenMemorySource(data.GetAsCArray(),length);
    CStaticMapInfoBlock info;
    ReadMapInfoBlock(info,stream);
    if(info.MapWidth==MapWidth && info.MapHeight==MapHeight && info.WorldX==WorldPosX && info.WorldY==WorldPosY) {
        CameraMapBounds=info.CameraMapBounds;
        if(state==STATIC_MAP_OPEN_STATE_OPEN_FOR_USE) {
            AsyncFileEntry=bank.GetAsyncEntry(index);
            FileBlockManager.SetBankFileEntry(AsyncFileEntry);
            stream.SetPosition(info.LandscapeMapPtr);
            LandscapeMap->OpenStaticMap(&FileBlockManager,stream);
            if(LocalDetailCacheMap) {
                stream.SetPosition(info.LocalDetailMapPtr);
                LocalDetailCacheMap->OpenStaticMap(&FileBlockManager,stream);
            }
        }
        stream.SetPosition(info.HeaderEndPtr);
        StaticMapOpenState=state;
    }
}
