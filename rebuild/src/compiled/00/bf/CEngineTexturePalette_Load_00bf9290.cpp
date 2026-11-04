#include "engine/CEngineTexturePalette.h"
#include "engine/CGraphicDataBank.h"
#include "fable_data_input_stream.h"

template<> void _STL::vector<CEngineTexturePalette::CPaletteEntry>::resize(size_t);

void CEngineTexturePalette::Load(CDataInputStream& stream)
{
    RetailVersion=stream.ReadValue<char>()!=0;
    if(RetailVersion) Palette.resize(0);
    else {
        long count=stream.ReadValue<long>();
        Palette.resize(count);
        Palette[0].Name=stream.ReadNullTerminatedString();
        Palette[0].Index=0;
        for(long i=1;i<count;++i) {
            Palette[i].Name=stream.ReadNullTerminatedString();
            unsigned long index=Bank->FindIndexBySymbol(Palette[i].Name);
            if(index>0) Palette[i].Index=index;
            else Palette[i].Index=DefaultIndex;
        }
    }
}
