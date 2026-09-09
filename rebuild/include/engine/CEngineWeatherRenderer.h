#ifndef FABLE_ENGINE_CENGINEWEATHERRENDERER_H
#define FABLE_ENGINE_CENGINEWEATHERRENDERER_H
// PDB-generated layout with parity-backed method declarations maintained here.
// Donor: Ego_r.pdb (ghidra_out/struct_layouts_egor.tsv) sizeof CEngineWeatherRenderer = 0x1328.
// Retail evidence: AddWeatherMask @ 0x00b50c80; reconciliation clean.
// Members are original Lionhead names; `_pad_*` are unnamed bytes; opaque
// `unsigned char X[N]` members carry their real (composite/template) type in a comment.
#include <stddef.h>
#include "rebuild_abi.h"
#include "C3DVector.h"
#include "CEngineWeatherSettings.h"
#include "CWeatherMask.h"

struct CVolumeTexture;
typedef long EWeatherBoxVisible; // PDB enum; enumerator names are not yet recovered.

#pragma pack(push, 1)
struct CEngineWeatherRenderer {
    void*           __vftable;                               // +0x00 vptr, or first dword of a flattened base subobject
    unsigned char   _pad_0x04[0x4];                          // +0x04
    unsigned char   _pad_0x08[0x4];                          // +0x08
    unsigned char   RainVertexBuffer[0x34];                  // +0x0c CVertexBufferWin32
    unsigned char   RainIndexBuffer[0x14];                   // +0x40 CIndexBuffer
    unsigned char   SnowVertexBuffer[0x34];                  // +0x54 CVertexBufferWin32
    unsigned char   MistVertexBuffer[0x34];                  // +0x88 CVertexBufferWin32
    unsigned char   MistIndexBuffer[0x14];                   // +0xbc CIndexBuffer
    unsigned char   RainParticleVertexShader[0x8];           // +0xd0 CVertexShader
    unsigned char   RainParticleClippingVertexShader[0x8];   // +0xd8 CVertexShader
    unsigned char   RainParticlePixelShader[0x8];            // +0xe0 CPixelShader
    unsigned char   SnowParticleVertexShader[0x8];           // +0xe8 CVertexShader
    unsigned char   SnowParticleClippingVertexShader[0x8];   // +0xf0 CVertexShader
    unsigned char   SnowParticlePixelShader[0x8];            // +0xf8 CPixelShader
    unsigned char   MistVertexShader[0x8];                   // +0x100 CVertexShader
    unsigned char   MistPixelShader[0x8];                    // +0x108 CPixelShader
    CVolumeTexture* MistVolumeTexture;                       // +0x110
    CEngineWeatherSettings Settings;                         // +0x114
    CWeatherMask    WeatherMaskList[3];                      // +0x66c
    long            CurrentMask;                             // +0x780
    EWeatherBoxVisible SavedBoxVisibility[729];              // +0x784
    float           OverridenRainStrength;                   // +0x12e8
    bool            RainOverriden;                           // +0x12ec
    unsigned char   _pad_0x12ed[0x3];                        // +0x12ed
    float           OverridenSnowStrength;                   // +0x12f0
    bool            SnowOverriden;                           // +0x12f4
    unsigned char   _pad_0x12f5[0x3];                        // +0x12f5
    float           MaximumWeatherParticlePercentage;        // +0x12f8
    bool            DrawDebugInfo;                           // +0x12fc
    bool            EnableSnowColours;                       // +0x12fd
    bool            DrawMaskSphere;                          // +0x12fe
    bool            DrawMaskBox;                             // +0x12ff
    bool            EnableDebugSpheres;                      // +0x1300
    bool            HideCameraClippedSpheres;                // +0x1301
    bool            HideBSClippedSpheres;                    // +0x1302
    bool            HideBBClippedSpheres;                    // +0x1303
    unsigned char   DebugSphereColour1[0x4];                 // +0x1304 CRGBColour
    unsigned char   DebugSphereColour2[0x4];                 // +0x1308 CRGBColour
    unsigned char   DebugSphereColour3[0x4];                 // +0x130c CRGBColour
    unsigned char   DebugSphereColour4[0x4];                 // +0x1310 CRGBColour
    bool            EnableDebugBoxes;                        // +0x1314
    bool            HideCameraClippedBoxes;                  // +0x1315
    bool            HideBSClippedBoxes;                      // +0x1316
    bool            HideBBClippedBoxes;                      // +0x1317
    unsigned char   DebugBoxColour1[0x4];                    // +0x1318 CRGBColour
    unsigned char   DebugBoxColour2[0x4];                    // +0x131c CRGBColour
    unsigned char   DebugBoxColour3[0x4];                    // +0x1320 CRGBColour
    unsigned char   DebugBoxColour4[0x4];                    // +0x1324 CRGBColour

    void AddWeatherMask(const CWeatherMask& mask, unsigned long flags);
    EWeatherBoxVisible ClipBoxToCamera(const C3DVector& position, float extent) const;
    EWeatherBoxVisible ClipLockedBoxToCamera(
        const C3DVector& position, float extent, long boxIndex);
};
#pragma pack(pop)

FABLE_STATIC_ASSERT(sizeof(CEngineWeatherRenderer) == 0x1328);
FABLE_STATIC_ASSERT(offsetof(CEngineWeatherRenderer, __vftable) == 0x0);
FABLE_STATIC_ASSERT(offsetof(CEngineWeatherRenderer, RainVertexBuffer) == 0xc);
FABLE_STATIC_ASSERT(offsetof(CEngineWeatherRenderer, RainIndexBuffer) == 0x40);
FABLE_STATIC_ASSERT(offsetof(CEngineWeatherRenderer, SnowVertexBuffer) == 0x54);
FABLE_STATIC_ASSERT(offsetof(CEngineWeatherRenderer, MistVertexBuffer) == 0x88);
FABLE_STATIC_ASSERT(offsetof(CEngineWeatherRenderer, MistIndexBuffer) == 0xbc);
FABLE_STATIC_ASSERT(offsetof(CEngineWeatherRenderer, RainParticleVertexShader) == 0xd0);
FABLE_STATIC_ASSERT(offsetof(CEngineWeatherRenderer, RainParticleClippingVertexShader) == 0xd8);
FABLE_STATIC_ASSERT(offsetof(CEngineWeatherRenderer, RainParticlePixelShader) == 0xe0);
FABLE_STATIC_ASSERT(offsetof(CEngineWeatherRenderer, SnowParticleVertexShader) == 0xe8);
FABLE_STATIC_ASSERT(offsetof(CEngineWeatherRenderer, SnowParticleClippingVertexShader) == 0xf0);
FABLE_STATIC_ASSERT(offsetof(CEngineWeatherRenderer, SnowParticlePixelShader) == 0xf8);
FABLE_STATIC_ASSERT(offsetof(CEngineWeatherRenderer, MistVertexShader) == 0x100);
FABLE_STATIC_ASSERT(offsetof(CEngineWeatherRenderer, MistPixelShader) == 0x108);
FABLE_STATIC_ASSERT(offsetof(CEngineWeatherRenderer, MistVolumeTexture) == 0x110);
FABLE_STATIC_ASSERT(offsetof(CEngineWeatherRenderer, Settings) == 0x114);
FABLE_STATIC_ASSERT(offsetof(CEngineWeatherRenderer, WeatherMaskList) == 0x66c);
FABLE_STATIC_ASSERT(offsetof(CEngineWeatherRenderer, CurrentMask) == 0x780);
FABLE_STATIC_ASSERT(offsetof(CEngineWeatherRenderer, SavedBoxVisibility) == 0x784);
FABLE_STATIC_ASSERT(offsetof(CEngineWeatherRenderer, OverridenRainStrength) == 0x12e8);
FABLE_STATIC_ASSERT(offsetof(CEngineWeatherRenderer, RainOverriden) == 0x12ec);
FABLE_STATIC_ASSERT(offsetof(CEngineWeatherRenderer, OverridenSnowStrength) == 0x12f0);
FABLE_STATIC_ASSERT(offsetof(CEngineWeatherRenderer, SnowOverriden) == 0x12f4);
FABLE_STATIC_ASSERT(offsetof(CEngineWeatherRenderer, MaximumWeatherParticlePercentage) == 0x12f8);
FABLE_STATIC_ASSERT(offsetof(CEngineWeatherRenderer, DrawDebugInfo) == 0x12fc);
FABLE_STATIC_ASSERT(offsetof(CEngineWeatherRenderer, EnableSnowColours) == 0x12fd);
FABLE_STATIC_ASSERT(offsetof(CEngineWeatherRenderer, DrawMaskSphere) == 0x12fe);
FABLE_STATIC_ASSERT(offsetof(CEngineWeatherRenderer, DrawMaskBox) == 0x12ff);
FABLE_STATIC_ASSERT(offsetof(CEngineWeatherRenderer, EnableDebugSpheres) == 0x1300);
FABLE_STATIC_ASSERT(offsetof(CEngineWeatherRenderer, HideCameraClippedSpheres) == 0x1301);
FABLE_STATIC_ASSERT(offsetof(CEngineWeatherRenderer, HideBSClippedSpheres) == 0x1302);
FABLE_STATIC_ASSERT(offsetof(CEngineWeatherRenderer, HideBBClippedSpheres) == 0x1303);
FABLE_STATIC_ASSERT(offsetof(CEngineWeatherRenderer, DebugSphereColour1) == 0x1304);
FABLE_STATIC_ASSERT(offsetof(CEngineWeatherRenderer, DebugSphereColour2) == 0x1308);
FABLE_STATIC_ASSERT(offsetof(CEngineWeatherRenderer, DebugSphereColour3) == 0x130c);
FABLE_STATIC_ASSERT(offsetof(CEngineWeatherRenderer, DebugSphereColour4) == 0x1310);
FABLE_STATIC_ASSERT(offsetof(CEngineWeatherRenderer, EnableDebugBoxes) == 0x1314);
FABLE_STATIC_ASSERT(offsetof(CEngineWeatherRenderer, HideCameraClippedBoxes) == 0x1315);
FABLE_STATIC_ASSERT(offsetof(CEngineWeatherRenderer, HideBSClippedBoxes) == 0x1316);
FABLE_STATIC_ASSERT(offsetof(CEngineWeatherRenderer, HideBBClippedBoxes) == 0x1317);
FABLE_STATIC_ASSERT(offsetof(CEngineWeatherRenderer, DebugBoxColour1) == 0x1318);
FABLE_STATIC_ASSERT(offsetof(CEngineWeatherRenderer, DebugBoxColour2) == 0x131c);
FABLE_STATIC_ASSERT(offsetof(CEngineWeatherRenderer, DebugBoxColour3) == 0x1320);
FABLE_STATIC_ASSERT(offsetof(CEngineWeatherRenderer, DebugBoxColour4) == 0x1324);

#endif // FABLE_ENGINE_CENGINEWEATHERRENDERER_H
