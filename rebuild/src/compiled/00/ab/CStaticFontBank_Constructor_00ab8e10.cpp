#include <new>
#include <string.h>
#include "fable_font_instances.h"
#include "fable_font_data_banks.h"
#include "fable_carray.h"
#include "fable_stlport_vector_impl.h"
#include "fable_data_input_stream.h"
#include "fable_gfuninitialise.h"
#include "fable_surface.h"
CStaticFontBank::CStaticFontBank(CFontDataBank* bank, unsigned long index)
{
    memset(GlyphBanks,0,sizeof(GlyphBanks));
    unsigned long entrySize=bank->GetEntryDataSize(index);
    CArray<char> entry(entrySize);
    bank->ReadEntryData(index,0,entry.GetAsCArray(),entrySize);
    CMemoryDataInputStream stream(entry.GetAsCArray(),entrySize);
    Name=stream.ReadNullTerminatedString();
    FontHeight=stream.ReadULONG();
    Weight=stream.ReadValue<long>();
    Italics=stream.ReadEBOOL();
    MaxHeight=stream.ReadULONG();
    TextureSize_X=stream.ReadValue<long>();
    TextureSize_Y=stream.ReadValue<long>();
    MinChar=stream.ReadValue<long>();
    MaxChar=stream.ReadValue<long>();
    unsigned long bankCount=stream.ReadULONG();
    for(unsigned long i=0;i<bankCount;++i) {
        unsigned long bankIndex=stream.ReadULONG();
        CStaticFontGlyphBank& glyphBank=GlyphBanks[bankIndex];
        glyphBank.FirstGlyph=static_cast<unsigned short>(stream.ReadULONG());
        glyphBank.NoGlyphs=static_cast<unsigned short>(stream.ReadULONG());
        glyphBank.Glyphs=new CGlyph[glyphBank.NoGlyphs];
        for(unsigned long j=0;j<glyphBank.NoGlyphs;++j)
            glyphBank.Glyphs[j].Read(stream);
    }
    unsigned long textureBytes=stream.ReadULONG();
    unsigned char textureHeader[18];
    stream.Read(textureHeader,18);
    CArray<char> pixels(textureBytes);
    stream.Read(pixels.GetAsCArray(),textureBytes-18);
    GFGetSystemManager();
    CPixelFormat rgba; rgba.Initialise(8,8,8,8,false);
    CPixelFormat compressed;
    GFGetSystemManager()->GetDisplayManager()->GetBestCompressedAlphaTextureFormat(16,compressed);
    PTexture=new CTexture(*reinterpret_cast<const C2DExtentsI*>(&TextureSize_X),1,compressed,0,SURFACE_POOL_MANAGED);
    C2DBoxI sourceBox;sourceBox.left=0;sourceBox.top=0;sourceBox.right=TextureSize_X;sourceBox.bottom=TextureSize_Y;
    C2DBoxI destinationBox=sourceBox;
    PTexture->GetSurfaceLevel(0).CopyFromMemory(&destinationBox,pixels.GetAsCArray(),&sourceBox,rgba,TextureSize_X*4,0xffffffff);
}
