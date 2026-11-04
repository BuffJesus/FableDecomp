#include "engine/CEngineLandscapeMap.h"
#include "fable_data_input_stream.h"
#include <new>
#include "fable_stlport_vector_impl.h"
template<> inline CCountedPointer<CBankFileAsyncEntry>::CCountedPointer(const CCountedPointer& other) : Data(other.Data),Info(other.Info) { if(Info)++Info->RefCount; }
template<> inline CCountedPointer<CBankFileAsyncEntry>& CCountedPointer<CBankFileAsyncEntry>::operator=(const CCountedPointer& other)
{
    CBankFileAsyncEntry* data=other.Data;CCPPointerInfo* info=other.Info;
    if(Info!=info){Release();Data=data;Info=info;if(Info)++Info->RefCount;}return *this;
}
void CEngineLandscapeMap::OpenStaticMap(CEngineStaticMapFileBlockManager* manager,CDataInputStream& stream)
{
    StaticMapFileBlockManager=manager;
    stream.Read(&MapFileHeader,sizeof(MapFileHeader));
    MapFileEntry=manager->GetBankFileEntry();
    CArrayVector<unsigned char> scratch(MapFileHeader.TexturePaletteSize);
    stream.SetPosition(MapFileHeader.TexturePaletteFilePointer);
    TexturePalette.Load(stream);
}
