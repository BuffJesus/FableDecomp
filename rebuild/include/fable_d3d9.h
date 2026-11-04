#ifndef FABLE_D3D9_H
#define FABLE_D3D9_H

// Direct3D 9 / D3DX declarations for engine sources. When the Windows SDK is
// already in the translation unit the real <d3d9.h> is used; otherwise the
// bootstrap (which builds with only VC7.1 + rebuild/include) gets these
// declarations in the SDK's shape: the same interfaces, methods and slot
// order, so COM calls compile to the retail `call [reg+slot]` sequences.

#if defined(_WINDOWS_) || defined(_D3D9_H_)

#include <d3d9.h>

#else

typedef long HRESULT;
typedef unsigned long DWORD;
typedef unsigned int UINT;
typedef int BOOL;
typedef float FLOAT;
typedef unsigned long D3DCOLOR;
typedef void* HANDLE;
typedef struct _GUID GUID;
typedef const GUID& REFIID;
typedef const GUID& REFGUID;

typedef struct tagRECT
{
    long left;
    long top;
    long right;
    long bottom;
} RECT;

typedef struct tagPALETTEENTRY PALETTEENTRY;

typedef enum _D3DFORMAT
{
    D3DFMT_UNKNOWN = 0,
    D3DFMT_R8G8B8 = 20,
    D3DFMT_A8R8G8B8 = 21,
    D3DFMT_X8R8G8B8 = 22,
    D3DFMT_A4R4G4B4 = 26,
    D3DFMT_A8 = 28,
    D3DFMT_INDEX16 = 101,
    D3DFMT_DXT1 = 0x31545844,       // MAKEFOURCC('D','X','T','1')
    D3DFMT_DXT3 = 0x33545844,       // MAKEFOURCC('D','X','T','3')
    D3DFMT_DXT5 = 0x35545844,       // MAKEFOURCC('D','X','T','5')
    D3DFMT_FORCE_DWORD = 0x7fffffff
} D3DFORMAT;

typedef enum _D3DRESOURCETYPE
{
    D3DRTYPE_SURFACE = 1,
    D3DRTYPE_TEXTURE = 3,
    D3DRTYPE_FORCE_DWORD = 0x7fffffff
} D3DRESOURCETYPE;

typedef enum _D3DPOOL
{
    D3DPOOL_DEFAULT = 0,
    D3DPOOL_MANAGED = 1,
    D3DPOOL_SYSTEMMEM = 2,
    D3DPOOL_SCRATCH = 3,
    D3DPOOL_FORCE_DWORD = 0x7fffffff
} D3DPOOL;

typedef enum _D3DMULTISAMPLE_TYPE
{
    D3DMULTISAMPLE_NONE = 0,
    D3DMULTISAMPLE_FORCE_DWORD = 0x7fffffff
} D3DMULTISAMPLE_TYPE;

typedef enum _D3DTEXTUREFILTERTYPE
{
    D3DTEXF_NONE = 0,
    D3DTEXF_FORCE_DWORD = 0x7fffffff
} D3DTEXTUREFILTERTYPE;

typedef struct _D3DSURFACE_DESC
{
    D3DFORMAT Format;
    D3DRESOURCETYPE Type;
    DWORD Usage;
    D3DPOOL Pool;
    D3DMULTISAMPLE_TYPE MultiSampleType;
    DWORD MultiSampleQuality;
    UINT Width;
    UINT Height;
} D3DSURFACE_DESC;

typedef struct _D3DVOLUME_DESC
{
    D3DFORMAT Format;
    D3DRESOURCETYPE Type;
    DWORD Usage;
    D3DPOOL Pool;
    UINT Width;
    UINT Height;
    UINT Depth;
} D3DVOLUME_DESC;

typedef struct _D3DLOCKED_BOX
{
    int RowPitch;
    int SlicePitch;
    void* pBits;
} D3DLOCKED_BOX;

typedef struct _D3DBOX
{
    UINT Left, Top, Right, Bottom, Front, Back;
} D3DBOX;

typedef struct _D3DLOCKED_RECT
{
    int Pitch;
    void* pBits;
} D3DLOCKED_RECT;

struct IDirect3DDevice9;
// d3d9types.h _D3DMATRIX (row-major _11.._44; the SDK's m[4][4] union view is
// not modelled). CRenderManagerCore::SetD3DMatrix 0x00A0AAD0 mangles it as
// U_D3DMATRIX.
typedef struct _D3DMATRIX
{
    float _11, _12, _13, _14;
    float _21, _22, _23, _24;
    float _31, _32, _33, _34;
    float _41, _42, _43, _44;
} D3DMATRIX;
struct D3DVIEWPORT9
{
    DWORD X, Y, Width, Height;
    float MinZ, MaxZ;
};

struct IUnknown
{
    virtual HRESULT __stdcall QueryInterface(REFIID riid, void** ppvObj) = 0;
    virtual unsigned long __stdcall AddRef() = 0;
    virtual unsigned long __stdcall Release() = 0;
};

struct IDirect3DResource9 : public IUnknown
{
    virtual HRESULT __stdcall GetDevice(IDirect3DDevice9** ppDevice) = 0;
    virtual HRESULT __stdcall SetPrivateData(
        REFGUID refguid, const void* pData, DWORD SizeOfData, DWORD Flags) = 0;
    virtual HRESULT __stdcall GetPrivateData(
        REFGUID refguid, void* pData, DWORD* pSizeOfData) = 0;
    virtual HRESULT __stdcall FreePrivateData(REFGUID refguid) = 0;
    virtual DWORD __stdcall SetPriority(DWORD PriorityNew) = 0;
    virtual DWORD __stdcall GetPriority() = 0;
    virtual void __stdcall PreLoad() = 0;
    virtual D3DRESOURCETYPE __stdcall GetType() = 0;
};

struct IDirect3DVolume9 : public IUnknown
{
    virtual HRESULT __stdcall GetDevice(IDirect3DDevice9** ppDevice) = 0;
    virtual HRESULT __stdcall SetPrivateData(
        REFGUID refguid, const void* pData, DWORD SizeOfData, DWORD Flags) = 0;
    virtual HRESULT __stdcall GetPrivateData(
        REFGUID refguid, void* pData, DWORD* pSizeOfData) = 0;
    virtual HRESULT __stdcall FreePrivateData(REFGUID refguid) = 0;
    virtual HRESULT __stdcall GetContainer(REFIID riid, void** ppContainer) = 0;
    virtual HRESULT __stdcall GetDesc(D3DVOLUME_DESC* pDesc) = 0;
    virtual HRESULT __stdcall LockBox(
        D3DLOCKED_BOX* pLockedVolume, const D3DBOX* pBox, DWORD Flags) = 0;
    virtual HRESULT __stdcall UnlockBox() = 0;
};

struct IDirect3DSurface9 : public IDirect3DResource9
{
    virtual HRESULT __stdcall GetContainer(REFIID riid, void** ppContainer) = 0;
    virtual HRESULT __stdcall GetDesc(D3DSURFACE_DESC* pDesc) = 0;
    virtual HRESULT __stdcall LockRect(
        D3DLOCKED_RECT* pLockedRect, const RECT* pRect, DWORD Flags) = 0;
    virtual HRESULT __stdcall UnlockRect() = 0;
    virtual HRESULT __stdcall GetDC(void** phdc) = 0;
    virtual HRESULT __stdcall ReleaseDC(void* hdc) = 0;
};

struct IDirect3DBaseTexture9 : public IDirect3DResource9
{
    virtual DWORD __stdcall SetLOD(DWORD LODNew) = 0;
    virtual DWORD __stdcall GetLOD() = 0;
    virtual DWORD __stdcall GetLevelCount() = 0;
    virtual HRESULT __stdcall SetAutoGenFilterType(
        D3DTEXTUREFILTERTYPE FilterType) = 0;
    virtual D3DTEXTUREFILTERTYPE __stdcall GetAutoGenFilterType() = 0;
    virtual void __stdcall GenerateMipSubLevels() = 0;
};

struct IDirect3DTexture9 : public IDirect3DBaseTexture9
{
    virtual HRESULT __stdcall GetLevelDesc(UINT Level, D3DSURFACE_DESC* pDesc) = 0;
    virtual HRESULT __stdcall GetSurfaceLevel(
        UINT Level, IDirect3DSurface9** ppSurfaceLevel) = 0;
    virtual HRESULT __stdcall LockRect(
        UINT Level, D3DLOCKED_RECT* pLockedRect, const RECT* pRect, DWORD Flags) = 0;
    virtual HRESULT __stdcall UnlockRect(UINT Level) = 0;
    virtual HRESULT __stdcall AddDirtyRect(const RECT* pDirtyRect) = 0;
};

struct IDirect3DVolumeTexture9 : public IDirect3DBaseTexture9
{
    virtual HRESULT __stdcall GetLevelDesc(UINT Level, D3DVOLUME_DESC* pDesc) = 0;
    virtual HRESULT __stdcall GetVolumeLevel(UINT Level, IDirect3DVolume9** ppVolumeLevel) = 0;
    virtual HRESULT __stdcall LockBox(
        UINT Level, D3DLOCKED_BOX* pLockedVolume, const D3DBOX* pBox, DWORD Flags) = 0;
    virtual HRESULT __stdcall UnlockBox(UINT Level) = 0;
    virtual HRESULT __stdcall AddDirtyBox(const D3DBOX* pDirtyBox) = 0;
};

// d3d9caps.h. CRenderManagerCore keeps one at +0x3A58 (Ego_r), so GFInitialise's
// reads of +0x3AB0/+0x3AB4 are D3DDeviceCaps.MaxTextureWidth/MaxTextureHeight.
typedef enum _D3DDEVTYPE
{
    D3DDEVTYPE_HAL = 1,
    D3DDEVTYPE_REF = 2,
    D3DDEVTYPE_SW = 3,
    D3DDEVTYPE_NULLREF = 4,
    D3DDEVTYPE_FORCE_DWORD = 0x7fffffff
} D3DDEVTYPE;

typedef struct _D3DVSHADERCAPS2_0
{
    DWORD Caps;
    int DynamicFlowControlDepth;
    int NumTemps;
    int StaticFlowControlDepth;
} D3DVSHADERCAPS2_0;

typedef struct _D3DPSHADERCAPS2_0
{
    DWORD Caps;
    int DynamicFlowControlDepth;
    int NumTemps;
    int StaticFlowControlDepth;
    int NumInstructionSlots;
} D3DPSHADERCAPS2_0;

typedef struct _D3DCAPS9
{
    D3DDEVTYPE DeviceType;
    UINT AdapterOrdinal;
    DWORD Caps;
    DWORD Caps2;
    DWORD Caps3;
    DWORD PresentationIntervals;
    DWORD CursorCaps;
    DWORD DevCaps;
    DWORD PrimitiveMiscCaps;
    DWORD RasterCaps;
    DWORD ZCmpCaps;
    DWORD SrcBlendCaps;
    DWORD DestBlendCaps;
    DWORD AlphaCmpCaps;
    DWORD ShadeCaps;
    DWORD TextureCaps;
    DWORD TextureFilterCaps;
    DWORD CubeTextureFilterCaps;
    DWORD VolumeTextureFilterCaps;
    DWORD TextureAddressCaps;
    DWORD VolumeTextureAddressCaps;
    DWORD LineCaps;
    DWORD MaxTextureWidth;
    DWORD MaxTextureHeight;
    DWORD MaxVolumeExtent;
    DWORD MaxTextureRepeat;
    DWORD MaxTextureAspectRatio;
    DWORD MaxAnisotropy;
    float MaxVertexW;
    float GuardBandLeft;
    float GuardBandTop;
    float GuardBandRight;
    float GuardBandBottom;
    float ExtentsAdjust;
    DWORD StencilCaps;
    DWORD FVFCaps;
    DWORD TextureOpCaps;
    DWORD MaxTextureBlendStages;
    DWORD MaxSimultaneousTextures;
    DWORD VertexProcessingCaps;
    DWORD MaxActiveLights;
    DWORD MaxUserClipPlanes;
    DWORD MaxVertexBlendMatrices;
    DWORD MaxVertexBlendMatrixIndex;
    float MaxPointSize;
    DWORD MaxPrimitiveCount;
    DWORD MaxVertexIndex;
    DWORD MaxStreams;
    DWORD MaxStreamStride;
    DWORD VertexShaderVersion;
    DWORD MaxVertexShaderConst;
    DWORD PixelShaderVersion;
    float PixelShader1xMaxValue;
    DWORD DevCaps2;
    float MaxNpatchTessellationLevel;
    DWORD Reserved5;
    UINT MasterAdapterOrdinal;
    UINT AdapterOrdinalInGroup;
    UINT NumberOfAdaptersInGroup;
    DWORD DeclTypes;
    DWORD NumSimultaneousRTs;
    DWORD StretchRectFilterCaps;
    D3DVSHADERCAPS2_0 VS20Caps;
    D3DPSHADERCAPS2_0 PS20Caps;
    DWORD VertexTextureFilterCaps;
    DWORD MaxVShaderInstructionsExecuted;
    DWORD MaxPShaderInstructionsExecuted;
    DWORD MaxVertexShader30InstructionSlots;
    DWORD MaxPixelShader30InstructionSlots;
} D3DCAPS9;

// d3d9types.h / d3d9.h subset for CDisplayManager (0x009BE420..0x009BEEB0):
// the interfaces keep the SDK's method order so `call [reg+slot]` matches.
// IDirect3DDevice9 is declared up to Clear (slot 43); later slots are not
// modelled yet (append in SDK order).
#define D3DCLEAR_TARGET  0x00000001l
#define D3DCLEAR_ZBUFFER 0x00000002l
#define D3DCLEAR_STENCIL 0x00000004l
#define D3DERR_DEVICELOST ((HRESULT)0x88760868L)

typedef struct _D3DDISPLAYMODE
{
    UINT Width;
    UINT Height;
    UINT RefreshRate;
    D3DFORMAT Format;
} D3DDISPLAYMODE;

typedef struct _D3DRECT
{
    long x1;
    long y1;
    long x2;
    long y2;
} D3DRECT;

struct HWND__;
struct HMONITOR__;
typedef struct _RGNDATA RGNDATA;
typedef struct tagPOINT POINT;
typedef struct _D3DADAPTER_IDENTIFIER9 D3DADAPTER_IDENTIFIER9;
typedef struct _D3DPRESENT_PARAMETERS_ D3DPRESENT_PARAMETERS;
typedef struct _D3DDEVICE_CREATION_PARAMETERS D3DDEVICE_CREATION_PARAMETERS;
typedef struct _D3DRASTER_STATUS D3DRASTER_STATUS;
typedef struct _D3DGAMMARAMP D3DGAMMARAMP;
struct IDirect3D9;
struct IDirect3DSwapChain9;
struct IDirect3DVolumeTexture9;
struct IDirect3DCubeTexture9;
struct IDirect3DVertexBuffer9;
struct IDirect3DIndexBuffer9;

struct IDirect3D9 : public IUnknown
{
    virtual HRESULT __stdcall RegisterSoftwareDevice(void* pInitializeFunction) = 0;
    virtual UINT __stdcall GetAdapterCount() = 0;
    virtual HRESULT __stdcall GetAdapterIdentifier(
        UINT Adapter, DWORD Flags, D3DADAPTER_IDENTIFIER9* pIdentifier) = 0;
    virtual UINT __stdcall GetAdapterModeCount(UINT Adapter, D3DFORMAT Format) = 0;
    virtual HRESULT __stdcall EnumAdapterModes(
        UINT Adapter, D3DFORMAT Format, UINT Mode, D3DDISPLAYMODE* pMode) = 0;
    virtual HRESULT __stdcall GetAdapterDisplayMode(UINT Adapter, D3DDISPLAYMODE* pMode) = 0;
    virtual HRESULT __stdcall CheckDeviceType(
        UINT Adapter, D3DDEVTYPE DevType, D3DFORMAT AdapterFormat,
        D3DFORMAT BackBufferFormat, BOOL bWindowed) = 0;
    virtual HRESULT __stdcall CheckDeviceFormat(
        UINT Adapter, D3DDEVTYPE DeviceType, D3DFORMAT AdapterFormat,
        DWORD Usage, D3DRESOURCETYPE RType, D3DFORMAT CheckFormat) = 0;
    virtual HRESULT __stdcall CheckDeviceMultiSampleType(
        UINT Adapter, D3DDEVTYPE DeviceType, D3DFORMAT SurfaceFormat,
        BOOL Windowed, D3DMULTISAMPLE_TYPE MultiSampleType, DWORD* pQualityLevels) = 0;
    virtual HRESULT __stdcall CheckDepthStencilMatch(
        UINT Adapter, D3DDEVTYPE DeviceType, D3DFORMAT AdapterFormat,
        D3DFORMAT RenderTargetFormat, D3DFORMAT DepthStencilFormat) = 0;
    virtual HRESULT __stdcall CheckDeviceFormatConversion(
        UINT Adapter, D3DDEVTYPE DeviceType, D3DFORMAT SourceFormat,
        D3DFORMAT TargetFormat) = 0;
    virtual HRESULT __stdcall GetDeviceCaps(
        UINT Adapter, D3DDEVTYPE DeviceType, D3DCAPS9* pCaps) = 0;
    virtual HMONITOR__* __stdcall GetAdapterMonitor(UINT Adapter) = 0;
    virtual HRESULT __stdcall CreateDevice(
        UINT Adapter, D3DDEVTYPE DeviceType, HWND__* hFocusWindow,
        DWORD BehaviorFlags, D3DPRESENT_PARAMETERS* pPresentationParameters,
        IDirect3DDevice9** ppReturnedDeviceInterface) = 0;
};

struct IDirect3DDevice9 : public IUnknown
{
    virtual HRESULT __stdcall TestCooperativeLevel() = 0;
    virtual UINT __stdcall GetAvailableTextureMem() = 0;
    virtual HRESULT __stdcall EvictManagedResources() = 0;
    virtual HRESULT __stdcall GetDirect3D(IDirect3D9** ppD3D9) = 0;
    virtual HRESULT __stdcall GetDeviceCaps(D3DCAPS9* pCaps) = 0;
    virtual HRESULT __stdcall GetDisplayMode(UINT iSwapChain, D3DDISPLAYMODE* pMode) = 0;
    virtual HRESULT __stdcall GetCreationParameters(
        D3DDEVICE_CREATION_PARAMETERS* pParameters) = 0;
    virtual HRESULT __stdcall SetCursorProperties(
        UINT XHotSpot, UINT YHotSpot, IDirect3DSurface9* pCursorBitmap) = 0;
    virtual void __stdcall SetCursorPosition(int X, int Y, DWORD Flags) = 0;
    virtual BOOL __stdcall ShowCursor(BOOL bShow) = 0;
    virtual HRESULT __stdcall CreateAdditionalSwapChain(
        D3DPRESENT_PARAMETERS* pPresentationParameters,
        IDirect3DSwapChain9** pSwapChain) = 0;
    virtual HRESULT __stdcall GetSwapChain(
        UINT iSwapChain, IDirect3DSwapChain9** pSwapChain) = 0;
    virtual UINT __stdcall GetNumberOfSwapChains() = 0;
    virtual HRESULT __stdcall Reset(D3DPRESENT_PARAMETERS* pPresentationParameters) = 0;
    virtual HRESULT __stdcall Present(
        const RECT* pSourceRect, const RECT* pDestRect,
        HWND__* hDestWindowOverride, const RGNDATA* pDirtyRegion) = 0;
    virtual HRESULT __stdcall GetBackBuffer(
        UINT iSwapChain, UINT iBackBuffer, int Type,
        IDirect3DSurface9** ppBackBuffer) = 0;
    virtual HRESULT __stdcall GetRasterStatus(
        UINT iSwapChain, D3DRASTER_STATUS* pRasterStatus) = 0;
    virtual HRESULT __stdcall SetDialogBoxMode(BOOL bEnableDialogs) = 0;
    virtual void __stdcall SetGammaRamp(
        UINT iSwapChain, DWORD Flags, const D3DGAMMARAMP* pRamp) = 0;
    virtual void __stdcall GetGammaRamp(UINT iSwapChain, D3DGAMMARAMP* pRamp) = 0;
    virtual HRESULT __stdcall CreateTexture(
        UINT Width, UINT Height, UINT Levels, DWORD Usage, D3DFORMAT Format,
        D3DPOOL Pool, IDirect3DTexture9** ppTexture, HANDLE* pSharedHandle) = 0;
    virtual HRESULT __stdcall CreateVolumeTexture(
        UINT Width, UINT Height, UINT Depth, UINT Levels, DWORD Usage,
        D3DFORMAT Format, D3DPOOL Pool, IDirect3DVolumeTexture9** ppVolumeTexture,
        HANDLE* pSharedHandle) = 0;
    virtual HRESULT __stdcall CreateCubeTexture(
        UINT EdgeLength, UINT Levels, DWORD Usage, D3DFORMAT Format, D3DPOOL Pool,
        IDirect3DCubeTexture9** ppCubeTexture, HANDLE* pSharedHandle) = 0;
    virtual HRESULT __stdcall CreateVertexBuffer(
        UINT Length, DWORD Usage, DWORD FVF, D3DPOOL Pool,
        IDirect3DVertexBuffer9** ppVertexBuffer, HANDLE* pSharedHandle) = 0;
    virtual HRESULT __stdcall CreateIndexBuffer(
        UINT Length, DWORD Usage, D3DFORMAT Format, D3DPOOL Pool,
        IDirect3DIndexBuffer9** ppIndexBuffer, HANDLE* pSharedHandle) = 0;
    virtual HRESULT __stdcall CreateRenderTarget(
        UINT Width, UINT Height, D3DFORMAT Format, D3DMULTISAMPLE_TYPE MultiSample,
        DWORD MultisampleQuality, BOOL Lockable, IDirect3DSurface9** ppSurface,
        HANDLE* pSharedHandle) = 0;
    virtual HRESULT __stdcall CreateDepthStencilSurface(
        UINT Width, UINT Height, D3DFORMAT Format, D3DMULTISAMPLE_TYPE MultiSample,
        DWORD MultisampleQuality, BOOL Discard, IDirect3DSurface9** ppSurface,
        HANDLE* pSharedHandle) = 0;
    virtual HRESULT __stdcall UpdateSurface(
        IDirect3DSurface9* pSourceSurface, const RECT* pSourceRect,
        IDirect3DSurface9* pDestinationSurface, const POINT* pDestPoint) = 0;
    virtual HRESULT __stdcall UpdateTexture(
        IDirect3DBaseTexture9* pSourceTexture,
        IDirect3DBaseTexture9* pDestinationTexture) = 0;
    virtual HRESULT __stdcall GetRenderTargetData(
        IDirect3DSurface9* pRenderTarget, IDirect3DSurface9* pDestSurface) = 0;
    virtual HRESULT __stdcall GetFrontBufferData(
        UINT iSwapChain, IDirect3DSurface9* pDestSurface) = 0;
    virtual HRESULT __stdcall StretchRect(
        IDirect3DSurface9* pSourceSurface, const RECT* pSourceRect,
        IDirect3DSurface9* pDestSurface, const RECT* pDestRect,
        D3DTEXTUREFILTERTYPE Filter) = 0;
    virtual HRESULT __stdcall ColorFill(
        IDirect3DSurface9* pSurface, const RECT* pRect, D3DCOLOR color) = 0;
    virtual HRESULT __stdcall CreateOffscreenPlainSurface(
        UINT Width, UINT Height, D3DFORMAT Format, D3DPOOL Pool,
        IDirect3DSurface9** ppSurface, HANDLE* pSharedHandle) = 0;
    virtual HRESULT __stdcall SetRenderTarget(
        DWORD RenderTargetIndex, IDirect3DSurface9* pRenderTarget) = 0;
    virtual HRESULT __stdcall GetRenderTarget(
        DWORD RenderTargetIndex, IDirect3DSurface9** ppRenderTarget) = 0;
    virtual HRESULT __stdcall SetDepthStencilSurface(IDirect3DSurface9* pNewZStencil) = 0;
    virtual HRESULT __stdcall GetDepthStencilSurface(IDirect3DSurface9** ppZStencilSurface) = 0;
    virtual HRESULT __stdcall BeginScene() = 0;
    virtual HRESULT __stdcall EndScene() = 0;
    virtual HRESULT __stdcall Clear(
        DWORD Count, const D3DRECT* pRects, DWORD Flags, D3DCOLOR Color,
        float Z, DWORD Stencil) = 0;
    virtual HRESULT __stdcall SetTransform(int State, const D3DMATRIX* matrix) = 0;
    virtual HRESULT __stdcall GetTransform(int State, D3DMATRIX* matrix) = 0;
    virtual HRESULT __stdcall MultiplyTransform(int State, const D3DMATRIX* matrix) = 0;
    virtual HRESULT __stdcall SetViewport(const D3DVIEWPORT9* viewport) = 0;
    // Slots 48-89 in SDK order (state enums as int, untyped structs as void);
    // CRenderManagerCore::DrawCurrentBuffersNonIndexed 0x00A0AD80 calls
    // SetTexture (65) and DrawPrimitive (81).
    virtual HRESULT __stdcall GetViewport(D3DVIEWPORT9* viewport) = 0;
    virtual HRESULT __stdcall SetMaterial(const void* material) = 0;
    virtual HRESULT __stdcall GetMaterial(void* material) = 0;
    virtual HRESULT __stdcall SetLight(DWORD Index, const void* light) = 0;
    virtual HRESULT __stdcall GetLight(DWORD Index, void* light) = 0;
    virtual HRESULT __stdcall LightEnable(DWORD Index, BOOL Enable) = 0;
    virtual HRESULT __stdcall GetLightEnable(DWORD Index, BOOL* pEnable) = 0;
    virtual HRESULT __stdcall SetClipPlane(DWORD Index, const float* pPlane) = 0;
    virtual HRESULT __stdcall GetClipPlane(DWORD Index, float* pPlane) = 0;
    virtual HRESULT __stdcall SetRenderState(int State, DWORD Value) = 0;
    virtual HRESULT __stdcall GetRenderState(int State, DWORD* pValue) = 0;
    virtual HRESULT __stdcall CreateStateBlock(int Type, void** ppSB) = 0;
    virtual HRESULT __stdcall BeginStateBlock() = 0;
    virtual HRESULT __stdcall EndStateBlock(void** ppSB) = 0;
    virtual HRESULT __stdcall SetClipStatus(const void* pClipStatus) = 0;
    virtual HRESULT __stdcall GetClipStatus(void* pClipStatus) = 0;
    virtual HRESULT __stdcall GetTexture(DWORD Stage, IDirect3DBaseTexture9** ppTexture) = 0;
    virtual HRESULT __stdcall SetTexture(DWORD Stage, IDirect3DBaseTexture9* pTexture) = 0;
    virtual HRESULT __stdcall GetTextureStageState(DWORD Stage, int Type, DWORD* pValue) = 0;
    virtual HRESULT __stdcall SetTextureStageState(DWORD Stage, int Type, DWORD Value) = 0;
    virtual HRESULT __stdcall GetSamplerState(DWORD Sampler, int Type, DWORD* pValue) = 0;
    virtual HRESULT __stdcall SetSamplerState(DWORD Sampler, int Type, DWORD Value) = 0;
    virtual HRESULT __stdcall ValidateDevice(DWORD* pNumPasses) = 0;
    virtual HRESULT __stdcall SetPaletteEntries(UINT PaletteNumber, const PALETTEENTRY* pEntries) = 0;
    virtual HRESULT __stdcall GetPaletteEntries(UINT PaletteNumber, PALETTEENTRY* pEntries) = 0;
    virtual HRESULT __stdcall SetCurrentTexturePalette(UINT PaletteNumber) = 0;
    virtual HRESULT __stdcall GetCurrentTexturePalette(UINT* PaletteNumber) = 0;
    virtual HRESULT __stdcall SetScissorRect(const RECT* pRect) = 0;
    virtual HRESULT __stdcall GetScissorRect(RECT* pRect) = 0;
    virtual HRESULT __stdcall SetSoftwareVertexProcessing(BOOL bSoftware) = 0;
    virtual BOOL __stdcall GetSoftwareVertexProcessing() = 0;
    virtual HRESULT __stdcall SetNPatchMode(float nSegments) = 0;
    virtual float __stdcall GetNPatchMode() = 0;
    virtual HRESULT __stdcall DrawPrimitive(int PrimitiveType, UINT StartVertex, UINT PrimitiveCount) = 0;
    virtual HRESULT __stdcall DrawIndexedPrimitive(int PrimitiveType, int BaseVertexIndex, UINT MinVertexIndex,
        UINT NumVertices, UINT startIndex, UINT primCount) = 0;
    virtual HRESULT __stdcall DrawPrimitiveUP(int PrimitiveType, UINT PrimitiveCount,
        const void* pVertexStreamZeroData, UINT VertexStreamZeroStride) = 0;
    virtual HRESULT __stdcall DrawIndexedPrimitiveUP(int PrimitiveType, UINT MinVertexIndex, UINT NumVertices,
        UINT PrimitiveCount, const void* pIndexData, D3DFORMAT IndexDataFormat, const void* pVertexStreamZeroData,
        UINT VertexStreamZeroStride) = 0;
    virtual HRESULT __stdcall ProcessVertices(UINT SrcStartIndex, UINT DestIndex, UINT VertexCount,
        void* pDestBuffer, void* pVertexDecl, DWORD Flags) = 0;
    virtual HRESULT __stdcall CreateVertexDeclaration(const void* pVertexElements, void** ppDecl) = 0;
    virtual HRESULT __stdcall SetVertexDeclaration(void* pDecl) = 0;
    virtual HRESULT __stdcall GetVertexDeclaration(void** ppDecl) = 0;
    virtual HRESULT __stdcall SetFVF(DWORD FVF) = 0;
};

#endif

typedef char FableD3DCaps9SizeMustBe304[sizeof(D3DCAPS9) == 304 ? 1 : -1];
typedef char FableD3DCaps9MaxTextureWidthAt58[
    (unsigned long)&((D3DCAPS9*)0)->MaxTextureWidth == 0x58 ? 1 : -1];

// d3dx9tex.h (statically linked D3DX, extern "C").
extern "C" HRESULT __stdcall D3DXLoadSurfaceFromSurface(
    IDirect3DSurface9* pDestSurface,
    const PALETTEENTRY* pDestPalette,
    const RECT* pDestRect,
    IDirect3DSurface9* pSrcSurface,
    const PALETTEENTRY* pSrcPalette,
    const RECT* pSrcRect,
    DWORD Filter,
    D3DCOLOR ColorKey);

#endif
