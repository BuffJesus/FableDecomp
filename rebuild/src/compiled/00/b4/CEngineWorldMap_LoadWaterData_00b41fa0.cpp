#include "engine/CEngineWorldMap.h"
#include "engine/CEngineWaterRenderer.h"
#include "fable_engine_internal.h"
#include "fable_bank_entry_update.h"
#include "fable_bank_async_entry.h"
#include "fable_data_input_stream.h"

extern CCharString WaterStaticMapBankFileName;

void CEngineWorldMap::LoadWaterData()
{
    unsigned long index = StaticMapFile.FindIndexBySymbol(WaterStaticMapBankFileName);
    if (index) {
        CBankFileEntryUpdateData* info = StaticMapFile.UpdateData[index];
        unsigned long infoSize = info->InfoSize;
        CArrayVector<char> infoBytes(infoSize);
        info->ReadInfo(0,infoBytes.GetAsCArray(),infoSize);
        CMemoryDataInputStream infoStream(infoBytes.GetAsCArray(),infoSize);
        CCountedPointer<CBankFileAsyncEntry> entry = StaticMapFile.GetAsyncEntry(index);
        unsigned long dataSize = entry->BankFile.GetEntryDataSize(entry->Index);
        CArrayVector<unsigned char> dataBytes(dataSize);
        entry->ReadDataNonAsync(0,dataBytes.GetAsCArray(),dataSize);
        CMemoryDataInputStream dataStream(dataBytes.GetAsCArray(),dataSize);
        NEngineInternal::GPWaterRenderer->LoadNonPatchData(dataStream,infoStream);
    }
}
