#include "fable_texture_lifecycle.h"

struct FableTextureSurfaceDescription
{
    fable_u32 format00;
    fable_u32 resourceType04;
    fable_u32 usage08;
    fable_u32 pool0C;
    fable_u32 multisampleType10;
    fable_u32 multisampleQuality14;
    fable_u32 width18;
    fable_u32 height1C;
};

struct FableByteLengthTexture
{
    virtual void Slot00() = 0;
    virtual void Slot04() = 0;
    virtual void Slot08() = 0;
    virtual void Slot0C() = 0;
    virtual void Slot10() = 0;
    virtual void Slot14() = 0;
    virtual void Slot18() = 0;
    virtual void Slot1C() = 0;
    virtual void Slot20() = 0;
    virtual void Slot24() = 0;
    virtual void Slot28() = 0;
    virtual void Slot2C() = 0;
    virtual void Slot30() = 0;
    virtual fable_u32 FABLE_STDCALL GetLevelCount() = 0;
    virtual void Slot38() = 0;
    virtual void Slot3C() = 0;
    virtual void Slot40() = 0;
    virtual fable_i32 FABLE_STDCALL GetLevelDescription(
        fable_u32 level,
        FableTextureSurfaceDescription* description) = 0;
};

FABLE_STATIC_ASSERT(sizeof(FableTextureSurfaceDescription) == 0x20);

void CTexturePreallocatedView::CalcByteLength()
{
    flags04 &= 0xF0000000;
    if (texture00 == 0)
    {
        return;
    }

    const fable_i32 levelCount =
        static_cast<fable_i32>(
            reinterpret_cast<FableByteLengthTexture*>(texture00)
                ->GetLevelCount());
    for (fable_i32 level = 0; level < levelCount; ++level)
    {
        FableTextureSurfaceDescription levelDescription;
        FableTextureSurfaceDescription baseDescription;
        reinterpret_cast<FableByteLengthTexture*>(texture00)
            ->GetLevelDescription(
            static_cast<fable_u32>(level),
            &levelDescription);
        reinterpret_cast<FableByteLengthTexture*>(texture00)
            ->GetLevelDescription(
            0,
            &baseDescription);

        CPixelFormatByteLengthView pixelFormat;
        pixelFormat.Initialise(baseDescription.format00);
        ByteLength +=
            pixelFormat.GetColourDepth() *
            levelDescription.height1C *
            levelDescription.width18 >>
            3;
    }
}
