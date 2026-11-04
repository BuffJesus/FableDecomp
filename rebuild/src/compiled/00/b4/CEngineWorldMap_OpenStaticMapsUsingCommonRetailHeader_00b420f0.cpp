#include "engine/CEngineWorldMap.h"
#include "fable_data_input_stream.h"
#include <new>
#include "fable_stlport_vector_impl.h"

void CEngineWorldMap::OpenStaticMapsUsingCommonRetailHeader(long index)
{
    unsigned long size = StaticMapFile.GetEntryDataSize(index);
    CArrayVector<unsigned char> bytes(size);
    StaticMapFile.ReadEntryData(index,0,bytes.GetAsCArray(),size);
    CMemoryDataInputStream stream(bytes.GetAsCArray(),bytes.size());
    _STL::map<CCharString,long> positions;
    long count = stream.ReadValue<long>();
    for (long i=0;i<count;++i) {
        CCharString name = stream.ReadNullTerminatedString();
        long position = stream.ReadValue<long>();
        positions.insert(_STL::make_pair(name,position));
    }
    for (unsigned i=0;i<MapList.size();++i) {
        if (MapList[i])
            MapList[i]->OpenRetailStaticMap(StaticMapFile,stream,positions);
    }
}
