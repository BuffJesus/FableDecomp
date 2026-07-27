#include <streams.h>
#include <stdio.h>

class FableTextureRendererLayoutProbe : public CBaseVideoRenderer
{
public:
    explicit FableTextureRendererLayoutProbe(HRESULT* result)
        : CBaseVideoRenderer(
            CLSID_NULL,
            TEXT("Fable Texture Renderer Layout Probe"),
            0,
            result),
          videoSystem160(0),
          texture164(0),
          textureFormat168(0),
          decodedWidth16C(0),
          decodedHeight170(0),
          sourceRowPitch174(0),
          ownsTexture178(0),
          unknown17C(0)
    {
    }

    HRESULT CheckMediaType(const CMediaType*)
    {
        return S_OK;
    }

    HRESULT DoRenderSample(IMediaSample*)
    {
        return S_OK;
    }

    void* videoSystem160;
    void* texture164;
    unsigned long textureFormat168;
    unsigned long decodedWidth16C;
    unsigned long decodedHeight170;
    unsigned long sourceRowPitch174;
    unsigned char ownsTexture178;
    unsigned char padding179[3];
    unsigned long unknown17C;
};

int main()
{
    HRESULT result = S_OK;
    FableTextureRendererLayoutProbe renderer(&result);
    printf(
        "FABLE_TEXTURE_RENDERER_LAYOUT size=0x%X result=0x%08X\n",
        static_cast<unsigned int>(sizeof(renderer)),
        static_cast<unsigned int>(result));
    return sizeof(renderer) == 0x180 && SUCCEEDED(result) ? 0 : 1;
}
