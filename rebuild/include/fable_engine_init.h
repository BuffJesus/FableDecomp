#pragma once

// CEngineInit: what a game component hands CIEngine::Initialise (slot 2).
// Ego_r.pdb names; retail layout (0x64) from CNewFrontendGameComponent::
// InitialiseEngine 0x0042E204 and CEngineInit's out-of-line ctor 0x0042D830
// / dtor 0x0042F7C1: retail has no DebugString (Ego_r +0x0C), so every
// later member sits 4 bytes lower, and CWideString is one pointer.

#include "fable_console.h"
#include "fable_string.h"
#include "rebuild_abi.h"

class CDefinitionManager;
class CEngineDef;
class CRenderManager2D;
class CIDrawWorld;
class CFontBank;
class CGraphicDataBank;
class CMeshDataBank;
class CIEngine;

class CEngineInit
{
public:
    CEngineInit();                                   // 0x0042D830
    ~CEngineInit();                                  // 0x0042F7C1

    CDefinitionManager* DefinitionManager;           // +0x00
    const CEngineDef* EngineDef;                     // +0x04
    CRenderManager2D* RenderManager;                 // +0x08 (Ego_r CRenderManager*)
    CIDrawWorld* World;                              // +0x0C
    const CFontBank* Font;                           // +0x10
    CGraphicDataBank* MainGraphicBank;               // +0x14
    CGraphicDataBank* TerrainGraphicBank;            // +0x18
    CMeshDataBank* MeshBank;                         // +0x1C
    CWideString MeshResourcePath;                    // +0x20
    CWideString GraphicsResourcePath;                // +0x24
    CWideString ShaderResourcePath;                  // +0x28
    CWideString EngineCacheResourcePath;             // +0x2C
    CWideString WriteableEngineCacheResourcePath;    // +0x30
    CEngineMemoryInit MemoryInit;                    // +0x34
    long StaticMapQuality;                           // +0x5C
    long InitFlags;                                  // +0x60
};

FABLE_STATIC_ASSERT(sizeof(CEngineInit) == 0x64);

// ?GFAllocEngine@@YIPAVCIEngine@@XZ (retail 0x00B26340).
CIEngine* FABLE_FASTCALL GFAllocEngine();

// ?GEngineMemoryInit@@3VCEngineMemoryInit@@A (retail 0x0137549C; named as in
// InitialiseConsoleVariables 0x00413C50).
extern CEngineMemoryInit g_FableEngineMemoryInit_0137549C;

// Donor InitEngine globals, mapped to retail loads in00435190.
extern long GStaticMapQuality;                 //01375490
extern bool GEnableMinimumStaticMapRebuild;    //013B8649
extern bool GAllowDataGeneration;              //01375459
extern bool GBuildRetailStaticMaps;            //013B864A
