#pragma once
#include <stddef.h>
#include "rebuild_abi.h"
#include "engine/CIndexBuffer.h"
#include "fable_font_vertex_buffer.h"
#include "fable_game_text_bank.h"
#include "fable_engine_component.h"
class CEngineVideoOptionsDef;
class CStaticMapBankFile;
class CMemoryDataInputStream;
// Partial retail view used by B6DB80. Donor offsets after the render-list
// arrays differ; this prefix is not a complete allocation-size claim.
class CEngineWaterRenderer : public CEngineComponent {
public:
    virtual bool SetEffectQuality(const CEngineVideoOptionsDef&);
    void CloseStaticMap();
    void OpenStaticMapForUse(CStaticMapBankFile*,CCharString);
    void LoadNonPatchData(CMemoryDataInputStream&,CMemoryDataInputStream&);
    void SetSeaBodyToRender(CCharString name) { RequestedSeaBodyID=name; }
    fable_u8 Unknown0C[0x68-0xc];
    CIndexBuffer SeaBackgroundIndexBuffer;
    CVertexBufferWin32 SeaBackgroundVertexBuffer;
    long SeaBackgroundVertexCount,SeaBackgroundPolyCount;
    fable_u8 UnknownB8[0x1bc];
    bool HighendWaterEnabled;
    bool HighendWaterSupported;
    bool HaveBackgroundSea;
    fable_u8 Unknown277;
    CCharString LoadedOrLoadingSeaBodyID,RequestedSeaBodyID;
    fable_u8 Unknown280[0x328];
    CStaticMapBankFile* StaticMapBankFile;
    CCharString StaticMapSeaEntryPrefix;
    CCountedPointer<CBankFileAsyncEntry> LoadingSeaEntry;
    CCountedPointer<CBankFileAsyncData> LoadingSea;
};
FABLE_STATIC_ASSERT(offsetof(CEngineWaterRenderer,SeaBackgroundIndexBuffer)==0x68);
FABLE_STATIC_ASSERT(offsetof(CEngineWaterRenderer,SeaBackgroundVertexBuffer)==0x7c);
FABLE_STATIC_ASSERT(offsetof(CEngineWaterRenderer,SeaBackgroundVertexCount)==0xb0);
FABLE_STATIC_ASSERT(offsetof(CEngineWaterRenderer,SeaBackgroundPolyCount)==0xb4);
FABLE_STATIC_ASSERT(offsetof(CEngineWaterRenderer,HaveBackgroundSea)==0x276);
FABLE_STATIC_ASSERT(offsetof(CEngineWaterRenderer,HighendWaterEnabled)==0x274);
FABLE_STATIC_ASSERT(offsetof(CEngineWaterRenderer,HighendWaterSupported)==0x275);
FABLE_STATIC_ASSERT(offsetof(CEngineWaterRenderer,RequestedSeaBodyID)==0x27c);
FABLE_STATIC_ASSERT(offsetof(CEngineWaterRenderer,StaticMapBankFile)==0x5a8);
FABLE_STATIC_ASSERT(offsetof(CEngineWaterRenderer,LoadingSeaEntry)==0x5b0);
FABLE_STATIC_ASSERT(offsetof(CEngineWaterRenderer,LoadingSea)==0x5b8);
