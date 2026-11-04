#ifndef FABLE_DISPLAY_VIEWPORT_H
#define FABLE_DISPLAY_VIEWPORT_H

#include "rebuild_abi.h"
#include "fable_d3d9.h"
#include "fable_d3d9_present.h"
#include "fable_extents_i.h"
#include "fable_carray.h"
#include "fable_render_state_manager.h"
#include "fable_stlport_list.h"
#include "fable_surface_types.h"
#include "fable_base_class.h"
#include "fable_string.h"
#include "fable_busy_resource_manager.h"
#include "fable_scoped_ptr.h"
#include "fable_quick_draw_info.h"
#include "fable_render2d_vertex.h"
#include "fable_state_block_function.h"
#include "fable_shader_data_bank.h"
#include "fable_vertex_shader.h"

class C2DExtentsI;
class CSurface;
class CDeviceResetCallback;
class CShaderResource;

// Ego_r.pdb layout (0x30); retail GetGameTimeOfNextPresentCompletion 0x004166E2
// receives it by value in a 0x30-byte slot and reads +0x10/+0x18/+0x28.
struct CVsyncCallbackTimerInfo
{
    // ??0CVsyncCallbackTimerInfo@@QAE@XZ (FableWin); retail keeps an out-of-line
    // copy at 0x009F7020 (zeroes the stamps and counts), called by
    // CVsyncCallbackTimer::GetInfo 0x009F7050.
    CVsyncCallbackTimerInfo()
        : CPUTimeStamp(0.0), CPUPrevTimeStamp(0.0), CPUFrameCount(0),
          GPUTimeStamp(0.0), GPUPrevTimeStamp(0.0), GPUFrameCount(0)
    {
    }

    double CPUTimeStamp;
    double CPUPrevTimeStamp;
    long CPUFrameCount;
    double GPUTimeStamp;
    double GPUPrevTimeStamp;
    long GPUFrameCount;
};

// kernel32 imports used by CVsyncCallbackTimer (retail IAT 0x0143FD34 / 0x0143FD38).
extern "C" __declspec(dllimport) long __stdcall InterlockedIncrement(volatile long* addend);
extern "C" __declspec(dllimport) long __stdcall InterlockedDecrement(volatile long* addend);

struct IDirect3DQuery9;
class CVsyncCallbackTimer
{
public:
    CVsyncCallbackTimer();
    ~CVsyncCallbackTimer();
    struct CPendingQuery { IDirect3DQuery9* Query; long Frame; double TimeStamp; };
    // ?GetInfo@CVsyncCallbackTimer@@QAE?AUCVsyncCallbackTimerInfo@@XZ, retail 0x009F7050.
    CVsyncCallbackTimerInfo GetInfo();
    // ?OnPostSwapScreens@CVsyncCallbackTimer@@QAEXXZ, retail 0x009F73C0
    // (caller CDisplayManager::SwapScreens 0x009BEEB0).
    void OnPostSwapScreens();
    void OnPreDeviceReset();
    void SetDevice(IDirect3DDevice9* device) { Device=device; }

    // FableWin.pdb CVsyncCallbackTimer::CGPUInfoBlock (0x18 bytes).
    struct CGPUInfoBlock
    {
        double GPUTimeStamp;
        double GPUPrevTimeStamp;
        long GPUFrameCount;
        long Locked;
    };

private:
    friend struct CVsyncCallbackTimerTestAccess;
    IDirect3DDevice9* Device;
    _STL::list<CPendingQuery> FreeQueoryObjects;
    _STL::list<CPendingQuery> PendingQueories;
    long PendingFrame, FinishedFrame;
    void ClearAllQueries(); // FableWin; retail 009F7250.
    // FableWin.pdb private statics; retail addresses from GetInfo 0x009F7050.
    static volatile CGPUInfoBlock GPUBlocks[2];   // 0x013CB4B0
    static double CPUTimeStamp;                   // 0x013CB4E0
    static double CPUPrevTimeStamp;               // 0x013CB4E8
    static long CPUFrameCount;                    // 0x013CB4F0
    static volatile long LatestBlockIndex;        // 0x013CB4F4
};

FABLE_STATIC_ASSERT(sizeof(CVsyncCallbackTimer)==0x14);
FABLE_STATIC_ASSERT(sizeof(CVsyncCallbackTimer::CPendingQuery)==16);
FABLE_STATIC_ASSERT(offsetof(CVsyncCallbackTimer::CPendingQuery,TimeStamp)==8);
class CSystemManager;
class CDisplayManagerInit;
class CPixelFormat;

class CRGBColour;
class CRenderTarget;
class CFontBank;
class C3DVector;
class C2DVector;

// FableWin ?ClearRenderTarget@CRenderManager2D@@QAEXABVCRGBColour@@K@Z (0x009D8CF0).
// RTTI CRenderManager2D : CRenderManagerCore : CRenderStateManager. The first
// 0x3A58 bytes (formerly an opaque pad) are now the two modelled bases,
// layout-identical: D3DDeviceCaps stays at +0x3A58, inherited from
// CRenderManagerCore (fable_render_state_manager.h).
class CRenderManager2D : public CRenderManagerCore
{
    friend struct CRenderManager2DTestAccess;
public:
    CRenderManager2D(CDisplayManager& display, const CWideString& shaderPath, unsigned long flags);
    // Implicit virtual destructor: retail 009C2FD0 performs member/base
    // teardown without writing a CRenderManager2D vtable first.
    void ClearRenderTarget(const CRGBColour& colour, unsigned long flags);
    // ?DrawBufferedPolys@CRenderManager2D@@QAEXXZ (retail 0x009D9C80) and
    // ?Render2DDrawList@CRenderManager2D@@QAEX_N@Z (retail 0x009DA9F0);
    // caller CNewFrontendGameComponent::Draw 0x0042DF9E.
    void DrawBufferedPolys();
    static CStateBlockFunctionBase* LibraryStateBlockFunctions[7];
    void Render2DDrawList(bool flag);
private:
    // ?Add2DTri@CRenderManager2D@@AAEXABVCTVertexRHWColSpecTex1@@00JPAVCTexture@@@Z, retail 009DC030.
    void Add2DTri(const CTVertexRHWColSpecTex1&, const CTVertexRHWColSpecTex1&, const CTVertexRHWColSpecTex1&, long, CTexture*);
    // Ego_r AddTri / AddAlphaTri / AddLine, retail 009DBD10 / 009DB810 / 009DBAE0.
    void AddTri(const CTVertexRHWColSpecTex1&, const CTVertexRHWColSpecTex1&, const CTVertexRHWColSpecTex1&, long, CTexture*);
    void AddAlphaTri(const CTVertexRHWColSpecTex1&, const CTVertexRHWColSpecTex1&, const CTVertexRHWColSpecTex1&, long, CTexture*);
    void AddLine(const CTVertexRHWColSpecTex1&, const CTVertexRHWColSpecTex1&, long, CTexture*);
    // ?Add2DLine@CRenderManager2D@@AAEXABVCTVertexRHWColSpecTex1@@0JPAVCTexture@@@Z, retail 009DC2C0.
    void Add2DLine(const CTVertexRHWColSpecTex1&, const CTVertexRHWColSpecTex1&, long, CTexture*);
public:
    // Ego_r AddLineToDrawBuffer / AddTriToDrawBuffer / AddQuadToDrawBuffer,
    // retail 009DC4C0 / 009DC4E0 / 009DC500.
    void AddLineToDrawBuffer(const CTVertexRHWColSpecTex1&, const CTVertexRHWColSpecTex1&, unsigned long, CTexture*);
    void AddTriToDrawBuffer(const CTVertexRHWColSpecTex1&, const CTVertexRHWColSpecTex1&, const CTVertexRHWColSpecTex1&, unsigned long, CTexture*);
    void AddQuadToDrawBuffer(const CTVertexRHWColSpecTex1&, const CTVertexRHWColSpecTex1&, const CTVertexRHWColSpecTex1&, const CTVertexRHWColSpecTex1&, unsigned long, CTexture*);
    // Ego_r Draw2DBoxWithZ / Draw2DBox, retail 009DDC60 / 009DE3F0.
    bool Draw2DBoxWithZ(const C2DBoxF& box, float z, const CRGBColour& colour, unsigned long flags);
    bool Draw2DBox(const C2DBoxF& box, const CRGBColour& colour, unsigned long flags);
    // Ego_r Draw2DLine / Draw2DCircle, retail 009DD960 / 009DE2A0.
    bool Draw2DLine(const C2DVector& from, const C2DVector& to, const CRGBColour& colour, unsigned long flags);
    bool Draw2DCircle(const C2DVector& centre, float radius, const CRGBColour& colour, unsigned long flags);
    // Ego_r Draw2DBoxOutline / Draw2DBoxColours, retail 009DE140 / 009DDA50.
    bool Draw2DBoxOutline(const C2DBoxF& box, const CRGBColour& colour, unsigned long flags);
    bool Draw2DBoxColours(const C2DBoxF& box, const CRGBColour* const colours, unsigned long flags);
    // Ego_r ?Draw2DTexture@CRenderManager2D@@QAE_NABVC2DBoxF@@ABVCRGBColour@@AAVCTexture@@K@Z, retail 009DC6E0.
    bool Draw2DTexture(const C2DBoxF& box, const CRGBColour& colour, CTexture& texture, unsigned long flags);
    // Ego_r Draw2DTriangle, retail 009DDFF0.
    bool Draw2DTriangle(const C2DVector& p, const C2DVector& q, const C2DVector& r, const CRGBColour& colour, unsigned long flags);
private:
public:
    bool Draw3DScaledText(const C3DVector& position, float scale,
        CWideString text, const CFontBank& font, const CRGBColour& colour,
        unsigned long flags); // retail 009DBFF0
    // Ego_r ?Draw2DText@CRenderManager2D@@QAE_NABVC2DVector@@VCWideString@@ABVCFontBank@@ABVCRGBColour@@K@Z, retail 009DD8F0.
    bool Draw2DText(const C2DVector& position, CWideString text, const CFontBank& font,
        const CRGBColour& colour, unsigned long flags);
private:
    void AddText(const C3DVector& position, float scale,
        const CWideString& text, const CFontBank& font,
        const CRGBColour& colour, unsigned long flags); // retail 009DB700
public:
    // Native CDisplayManager::Init allocates 0x3EA4 for CRenderManager.
    // 2D ctor 009D9A30; the retail bank omits donor parser members.
    CShaderDataBank ShaderDataBank;
    CCountedPointer<CShaderResource> ShaderResource;
    CVertexShader VertexShader2D;
    // PDB member order, adjusted for retail's 12-byte STLport vectors.
    _STL::vector<CTVertexRHWColSpecTex1> QuickDrawVertices;
    _STL::vector<CQuickDrawTriInfo> QuickDrawTriInfo;
    _STL::vector<CTVertexRHWColSpecTex1> QuickDrawAlphaVertices;
    _STL::vector<CQuickDrawTriInfo> QuickDrawAlphaTriInfo;
    _STL::vector<CTVertexRHWColSpecTex1> QuickDraw2DVertices;
    _STL::vector<CQuickDrawTriInfo> QuickDraw2DTriInfo;
    bool AlternateRenderTargetClearEnabled;
    unsigned char unknown3EA1_[3];
};

class CRenderManager : public CRenderManager2D {
public:
    CRenderManager(CDisplayManager& display,const CWideString& shaderPath,unsigned long flags)
        : CRenderManager2D(display,shaderPath,flags) {}
    // Implicit destructor folds to the same member teardown as the 2D base.
};
FABLE_STATIC_ASSERT(sizeof(CRenderManager2D)==0x3ea4);
FABLE_STATIC_ASSERT(offsetof(CRenderManager2D,ShaderDataBank)==0x3d20);
FABLE_STATIC_ASSERT(offsetof(CRenderManager2D,ShaderResource)==0x3e48);
FABLE_STATIC_ASSERT(offsetof(CRenderManager2D,VertexShader2D)==0x3e50);
FABLE_STATIC_ASSERT(offsetof(CRenderManager2D,QuickDrawVertices)==0x3e58);
FABLE_STATIC_ASSERT(offsetof(CRenderManager2D,QuickDrawTriInfo)==0x3e64);
FABLE_STATIC_ASSERT(offsetof(CRenderManager2D,QuickDrawAlphaVertices)==0x3e70);
FABLE_STATIC_ASSERT(offsetof(CRenderManager2D,QuickDrawAlphaTriInfo)==0x3e7c);
FABLE_STATIC_ASSERT(offsetof(CRenderManager2D,QuickDraw2DVertices)==0x3e88);
FABLE_STATIC_ASSERT(sizeof(CRenderManager)==0x3ea4);

class CDisplayManager : public CBaseClassNonCopyable
{
    friend struct CDisplayManagerTestAccess;
    void DestroyDevice();
    void InitVirtualPixels();
    void PreDeviceReset();
    bool PostDeviceReset();
public:
    // 0x9C0880 / 0x9C0E50 (CSystemManager::Initialise builds and inits it).
    explicit CDisplayManager(CSystemManager& systemManager);
    virtual ~CDisplayManager();
    void Init(const CDisplayManagerInit& init, bool exclusiveMode);
    void ResizeWindowToBackBufferSize();

    const C2DExtentsI GetRenderTargetDimensions();
    unsigned long GetPixelShaderVersion() const; // retail 009BEE00
    void SetViewport(const C2DBoxF& viewport);
    // Ego_r SetViewport(const C2DBoxI&) (retail 0x009BEF80), SetRenderTarget /
    // SetRenderTargetAsBackScreen (retail 0x009BF160 / 0x009BF220).
    void SetViewport(const C2DBoxI& viewport);
    bool SetRenderTarget(CRenderTarget& target);
    bool SetRenderTargetAsBackScreen();
    void AddDeviceResetCallback(CDeviceResetCallback* callback);
    // ?RemoveDeviceResetCallback@CDisplayManager@@QAEXPAVCDeviceResetCallback@@@Z
    // (retail 0x009BF5A0; caller CMainGameComponent::Shutdown 0x004175E5).
    void RemoveDeviceResetCallback(CDeviceResetCallback* callback);
    // ?GetBackBuffer@CDisplayManager@@QAEAAVCSurface@@XZ: inline in retail
    // (`GPDisplayManager + 0x2C`, CMainGameComponent::InitDisplayEngine
    // 0x00417418); defined in fable_surface.h, where CSurface is complete.
    CSurface& GetBackBuffer();
    // ?GetDepthBufferFormat@CDisplayManager@@QAEABVCPixelFormat@@XZ; defined in
    // fable_surface.h, where CPixelFormat is complete.
    const CPixelFormat& GetDepthBufferFormat();

    // Link-visible name for the recovered retail integer overload at
    // 0x009BEF80. C2DBoxI stores inclusive right/bottom edges; the endpoint
    // converts them to exclusive D3D extents after clamping against the
    // active render target.
    void SetIntegerViewportEndpoint(const C2DBoxI& viewport);

    // FableWin.pdb names; retail addresses from
    // CNewFrontendGameComponent::InitialiseGraphics 0x0042DDB3.
    bool GetBestCompressedOpaqueTextureFormat(unsigned long bitDepth, CPixelFormat& format);        // 0x009BE830
    bool GetBestCompressedAlphaTextureFormat(unsigned long bitDepth, CPixelFormat& format);         // 0x009BE870
    bool GetBestCompressedBooleanAlphaTextureFormat(unsigned long bitDepth, CPixelFormat& format);  // 0x009BE8B0
    bool GetBestAlphaTextureFormat(unsigned long bitDepth, CPixelFormat& format);                   // 0x009BE6C0
    bool GetBestOpaqueTextureFormat(unsigned long bitDepth, CPixelFormat& format, bool renderTarget);     // 0x009BE610
    bool GetBestSignedTextureFormat(unsigned long bitDepth, CPixelFormat& format);                  // 0x009BE590
    // Caller: CMainGameComponent::InitGraphics 0x00416C8A.
    bool GetBestBooleanAlphaTextureFormat(unsigned long bitDepth, CPixelFormat& format);            // 0x009BE750
    bool IsValidTextureFormat(const CPixelFormat&);
    bool IsValidRenderTargetFormat(const CPixelFormat&, const CPixelFormat&);
    // Ego_r ?IsAntiAliasingModeValid@CDisplayManager@@QAE_NW4ESurfaceMultisampleType@@ABVCPixelFormat@@@Z,
    // retail 0x009BE3C0.
    bool IsAntiAliasingModeValid(ESurfaceMultisampleType type, const CPixelFormat& format);
    bool IsSupportingCompressedTextures();                                                          // 0x009BE800 ?IsSupportingCompressedTextures@CDisplayManager@@QAE_NXZ

    // FableWin.pdb ?ClearRenderTarget@CDisplayManager@@QAEXABVCRGBColour@@K@Z (0x009BE420)
    // and ?SwapScreens@CDisplayManager@@QAE_NXZ (0x009BEEB0); callers:
    // CNewFrontendGameComponent::Shutdown 0x0042EBB6.
    void ClearRenderTarget(const CRGBColour& colour, unsigned long flags);
    bool SwapScreens();
    // Ego_r ?FlipToGDISurface@CDisplayManager@@QAEXXZ, retail 0x009BF2D0.
    void FlipToGDISurface();
    // FableWin SetGamma(float,bool) / GetGamma() const; retail 009BEAA0 and
    // inline WinProcedure read at 009A5C4D. Ego_r Gamma +0x2E4 maps to +0x2C4.
    void SetGamma(float gamma, bool calibrate);
    float GetGamma() const { return Gamma; }

    // FableWin ?ProcessLostDevice@CDisplayManager@@QAE_NXZ (retail 0x009C00C0;
    // caller CSystemManager::UpdateActive 0x009A6370).
    bool ProcessLostDevice();

    // ?IsTextureBitDepthSupported@CDisplayManager@@QAE_NK@Z (0x009BE530).
    bool IsTextureBitDepthSupported(unsigned long bitDepth);

    // ?ChangePresentationInterval@CDisplayManager@@QAEX_NJ0@Z (0x009BFF10);
    // caller: CMainGameComponent::Run 0x004189C2.
    void ChangePresentationInterval(bool enable, long interval, bool apply);

    // Ego_r.pdb CDisplayManager::CPresentationIntervalDesc (8 bytes); retail
    // 0x009BFF10 builds one on the stack: +0 = arg 1, +1 = arg 3, +4 = arg 2.
    class CPresentationIntervalDesc
    {
    public:
        bool PresentImmediate;
        bool PresentImmediateIfSyncMissed;
        long VSyncCount;
    };
    // ?ChangePresentationInterval@CDisplayManager@@QAEXABVCPresentationIntervalDesc@1@@Z
    // (retail 0x009BFE70; called by the (bool, long, bool) overload).
    void ChangePresentationInterval(const CPresentationIntervalDesc& desc);

    // FableWin ?GetVVisibleTelevisionScreenArea@CDisplayManager@@QBE?AVC2DBoxF@@XZ
    // (retail 0x009BECE0), ?BeginRender@CDisplayManager@@QAE_NXZ (0x009BEF20)
    // and ?EndRender@CDisplayManager@@QAEXXZ (0x009BEF50); caller
    // CNewFrontendGameComponent::Draw 0x0042DF9E.
    C2DBoxF GetVVisibleTelevisionScreenArea() const;
    bool BeginRender();
    void EndRender();

    // FableWin.pdb enum CDisplayManager::ECreateDeviceSuccess.
    enum ECreateDeviceSuccess
    {
        CREATE_DEVICE_FAILED = 0,
        CREATE_DEVICE_DEVICE_LOST = 1,
        CREATE_DEVICE_SUCCESS = 2
    };

    struct CCreateDeviceParameters
    {
        CCreateDeviceParameters() {}
        CCreateDeviceParameters(const CCreateDeviceParameters& other)
            : WinHandle(other.WinHandle), BackBufferDimensions(other.BackBufferDimensions),
              BackBufferBitDepth(other.BackBufferBitDepth), ZBufferBitDepth(other.ZBufferBitDepth),
              BackBufferCount(other.BackBufferCount), RefreshRate(other.RefreshRate),
              ExclusiveMode(other.ExclusiveMode), PresentationInterval(other.PresentationInterval),
              MultisampleType(other.MultisampleType) {}
        HWND__* WinHandle;
        C2DExtentsI BackBufferDimensions;
        long BackBufferBitDepth, ZBufferBitDepth, BackBufferCount, RefreshRate;
        bool ExclusiveMode;
        CPresentationIntervalDesc PresentationInterval;
        ESurfaceMultisampleType MultisampleType;
    };
private:
    ECreateDeviceSuccess ResetDevice(const CCreateDeviceParameters& parameters);
    ECreateDeviceSuccess CreateDevice(const CCreateDeviceParameters& parameters);
    unsigned int CalculateD3DPresentationInterval(const CPresentationIntervalDesc& desc);
    bool SetupDepthAndBackBufferTextures();
    CPixelFormat GetClosestDepthBufferFormat(const D3DDISPLAYMODE& mode, long bitDepth) const;
    D3DDISPLAYMODE GetClosestMode(const C2DExtentsI& dimensions, long bitDepth, bool exclusive) const;
public:

    // ?SetDisplayMode@CDisplayManager@@QAE?AW4ECreateDeviceSuccess@1@ABVC2DExtentsI@@KW4ESurfaceMultisampleType@@K@Z
    // (retail 0x009BFF40; caller CNewFrontendGameComponent::Init2 0x0042E98F).
    // FableWin SetExclusiveMode(bool), retail 009C0020.
    ECreateDeviceSuccess SetExclusiveMode(bool exclusive);

    ECreateDeviceSuccess SetDisplayMode(const C2DExtentsI& dimensions,
                                        unsigned long bitDepth,
                                        ESurfaceMultisampleType multisample,
                                        unsigned long refreshRate);

public:
    // Ego_r lib_display_manager.cpp, retail 0x009BE400 / 0x009BE410 / 0x009BECB0 /
    // 0x009BED80 / 0x009BEDE0 (CDisplayManager_SmallQueries_009be380.cpp).
    void FlushVideoMemeory();
    long GetAvailableTextureMemory();
    C2DBoxF GetAVisibleTelevisionScreenArea() const;
    float ScaleForAntialiasing(float value);
    unsigned long GetVertexShaderVersion() const;
    // Ego_r ?SetVirtualCoordsAsResolutionIndependant@CDisplayManager@@QAEX_N@Z, retail 0x009BEA10.
    void SetVirtualCoordsAsResolutionIndependant(bool independant);
    // Ego_r ?BlitRenderTargetSurfaceToTexture@CDisplayManager@@QAEXAAVCTexture@@ABVCSurface@@@Z,
    // retail 0x009BF350; ?CopyBackBufferToTexture@CDisplayManager@@QAEAAVCTexture@@XZ, 0x009BF5E0.
    void BlitRenderTargetSurfaceToTexture(CTexture& texture, const CSurface& surface);
    CTexture& CopyBackBufferToTexture();
    // Empty on PC retail: 0x009BE330 / 0x009BEE20, and AllocTile 0x009BEE30
    // always returns 0.
    void ChangePresentationIntervalLockToGivenFrameRateOrLower(float frame_rate, bool lower);
    void ReleaseTile(long tile, CSurface& surface);
    long AllocTile(CSurface& surface, bool flag);
private:
    // Ego_r ?GetCurrentWindowedSettings@CDisplayManager@@AAEXPAUHWND__@@AAVC2DExtentsI@@AAJ@Z,
    // retail 0x009BED10.
    void GetCurrentWindowedSettings(HWND__* window, C2DExtentsI& dimensions, long& bit_depth);
public:
private:
    // Ego_r ?CalcPresentationIntervalTime@CDisplayManager@@AAEMJJ@Z, retail 0x009BE380.
    float CalcPresentationIntervalTime(long refresh_rate, long interval);
    // Ego_r ?ReadDeviceRefreshRate@CDisplayManager@@AAEJXZ, retail 0x009BE340.
    long ReadDeviceRefreshRate();
public:
    // Retail object is 0x2C8 bytes (operator new in CSystemManager::Initialise).
    // Ego_r names; retail agrees at +0x08 (GFInitialise caches it as the 2D render
    // manager) and at +0x1C8/+0x1D4 (CreateDevice 0x009BF7E0 stores RefreshRate,
    // then PresentationIntervalTime = interval / RefreshRate). Ego_r is 0x2EC
    // bytes; named retail members below use native corroborated offsets.
    CSystemManager* SystemManager; // Ego_r +4; reset phase callbacks confirm retail.
    boost::scoped_ptr<CRenderManager> PRenderManager; // PDB owning pointer, +0x08
    CSurface RenderTargetSurface; // CSurface +0x0C
    CSurface RenderTargetDepthBuffer; // CSurface +0x1C
    // Ego_r CSurface BackBuffer (+0x2C, 0x10 bytes).
    CSurface BackBuffer;
    // +0x3C..+0x1C8: Ego_r names/offsets, confirmed by retail 0x009BE420..0x009BEEB0
    // (D3D +0x54 / D3DDevice +0x58 / caps DeviceType +0x5C, AdapterOrdinal +0x60 fed
    // to CheckDeviceFormat; DepthBufferFormat +0x1AC; CurrentMode.Format +0x1C4;
    // NoBackBuffers +0x19C in SwapScreens).
    CSurface DepthBuffer; // CSurface +0x3C
    CTexture BackBufferCopyTexture; // CTexture +0x4C
    IDirect3D9* D3D;                        // +0x54
    IDirect3DDevice9* D3DDevice;            // +0x58
    D3DCAPS9 D3DDeviceCaps;                 // +0x5C
    // C2DExtentsI BackBufferDimensions (+0x18C) / RenderTargetDimensions (+0x194):
    // Dimension storage retains its existing array accessors.
    long BackBufferDimensionsStorage_[2];
    long RenderTargetDimensionsStorage_[2];
    // Ego_r C2DExtentsI RenderTargetDimensions (+0x194) as a whole value
    // (SetRenderTarget 0x009BF160 assigns GetDimensions() straight into it).
    C2DExtentsI& RenderTargetDimensions()
    {
        return *reinterpret_cast<C2DExtentsI*>(RenderTargetDimensionsStorage_);
    }
    long NoBackBuffers;                     // +0x19C
    bool ExclusiveMode;                     // +0x1A0
    ESurfaceMultisampleType MultisampleType; // +0x1A4
    // CPixelFormat BackBufferFormat (+0x1A8) / DepthBufferFormat (+0x1AC): one
    // long FormatIndex each; typed through GetBackBufferFormat() /
    // GetDepthBufferFormat() in fable_surface.h, where CPixelFormat is complete.
    long BackBufferFormatStorage_;
    long DepthBufferFormatStorage_;
    bool UsingRenderManager;                // +0x1B0
    bool UsingShaderRenderManager;          // +0x1B1
    HWND__* WindowHandle;                   // +0x1B4
    D3DDISPLAYMODE CurrentMode;             // +0x1B8 (Format at +0x1C4)
    long RefreshRate;
    CPresentationIntervalDesc PresentationInterval; // +0x1CC
    // Ego_r +0x1D4 too; CMainGameComponent::GetGameTimeOfNextPresentCompletion
    // 0x004166E2 multiplies the pending-frame count by it.
    float PresentationIntervalTime;
    bool ViewportOffscreen; // +0x1D8, PDB and AddText 009DB700.
    fable_u8 unknown1D9_[3];
    D3DVIEWPORT9 D3DViewport;
    C2DBoxI ViewportBox;
    C2DBoxF ViewportBoxFloat;
    bool CanBeginRender; // +0x214
    fable_u8 unknown215_[3];
    long CurrentScreenIndex;                // +0x218 (SwapScreens 0x009BEEB0)
    D3DPRESENT_PARAMETERS PresentParams; // +0x21C
    bool VirtualCoordsResolutionIndependant; // +0x254
    bool DeviceLostFlag;                    // +0x255 (SwapScreens: Present == D3DERR_DEVICELOST)
    fable_u8 unknown256_[2];
    CWideString ShaderResourcePath; // +0x258
    long FrameCounter;                      // +0x25C (SwapScreens)
    unsigned long RenderManagerInitFlags; // +0x260
    // PDB DeviceResetCallbacks: retail STLport list stores only its sentinel
    // pointer. Constructor owns the sentinel; destructor recovery is pending.
    _STL::list<CDeviceResetCallback*> DeviceResetCallbacks; // +0x264
    CCreateDeviceParameters LastDeviceCreationParameters; // +0x268, PDB name/native copies.
    // Ego_r: scoped_ptr<CVsyncCallbackTimer> at +0x29C; retail reads it at
    // +0x294 (0x004166E2), 8 bytes lower like the rest of the tail.
    boost::scoped_ptr<CVsyncCallbackTimer> VsyncCallbackTimer;
    CArrayVector<CPixelFormat> BackBufferFormats; // +0x298, PDB CArray in retail STLport layout.
    CArrayVector<ESurfaceMultisampleType> MultisampleTypes; // +0x2A4
    CBusyResourceManager BusyResourceManager; // +0x2B0
    float Gamma; // Ego_r +0x2E4; retail WinProcedure reads +0x2C4.

    // ?GetVsyncCallbackTimer@CDisplayManager@@QAEAAVCVsyncCallbackTimer@@XZ; inline in retail.
    CVsyncCallbackTimer& GetVsyncCallbackTimer()
    {
        return *VsyncCallbackTimer;
    }

    // FableWin ?GetRenderManager@CDisplayManager@@QAEAAVCRenderManager@@XZ;
    // inline in retail (CNewFrontendGameComponent::InitialiseEngine 0x0042E204
    // reads +0x08). Typed as the 2D manager this header models.
    CRenderManager& GetRenderManager()
    {
        return *PRenderManager;
    }
};

typedef char FableC2DBoxFSizeMustBe16[
    sizeof(C2DBoxF) == 0x10 ? 1 : -1];
typedef char FableCVsyncCallbackTimerInfoSizeMustBe30[
    sizeof(CVsyncCallbackTimerInfo) == 0x30 ? 1 : -1];
typedef char FableCDisplayManagerSizeMustBe2C8[
    sizeof(CDisplayManager) == 0x2C8 ? 1 : -1];
typedef char FableCDisplayManagerD3DDeviceAt58[
    (unsigned long)&((CDisplayManager*)0)->D3DDevice == 0x58 ? 1 : -1];
typedef char FableCDisplayManagerNoBackBuffersAt19C[
    (unsigned long)&((CDisplayManager*)0)->NoBackBuffers == 0x19C ? 1 : -1];
typedef char FableCDisplayManagerDepthBufferFormatAt1AC[
    (unsigned long)&((CDisplayManager*)0)->DepthBufferFormatStorage_ == 0x1AC ? 1 : -1];
typedef char FableCDisplayManagerCurrentModeAt1B8[
    (unsigned long)&((CDisplayManager*)0)->CurrentMode == 0x1B8 ? 1 : -1];
typedef char FableCDisplayManagerCurrentScreenIndexAt218[
    (unsigned long)&((CDisplayManager*)0)->CurrentScreenIndex == 0x218 ? 1 : -1];
typedef char FableCDisplayManagerDeviceLostFlagAt255[
    (unsigned long)&((CDisplayManager*)0)->DeviceLostFlag == 0x255 ? 1 : -1];
typedef char FableCDisplayManagerFrameCounterAt25C[
    (unsigned long)&((CDisplayManager*)0)->FrameCounter == 0x25C ? 1 : -1];
typedef char FableC2DBoxISizeMustBe16[
    sizeof(C2DBoxI) == 0x10 ? 1 : -1];

typedef char FableCDisplayManagerGammaAt2C4[
    (unsigned long)&((CDisplayManager*)0)->Gamma == 0x2C4 ? 1 : -1];

FABLE_STATIC_ASSERT(offsetof(CDisplayManager,DeviceResetCallbacks)==0x264);
#endif

FABLE_STATIC_ASSERT(sizeof(CDisplayManager::CCreateDeviceParameters)==44);
FABLE_STATIC_ASSERT(offsetof(CDisplayManager,LastDeviceCreationParameters)==0x268);

FABLE_STATIC_ASSERT(sizeof(D3DPRESENT_PARAMETERS)==56);
FABLE_STATIC_ASSERT(offsetof(CDisplayManager,PresentParams)==0x21C);

FABLE_STATIC_ASSERT(offsetof(CDisplayManager,RenderTargetSurface)==0x0c);
FABLE_STATIC_ASSERT(offsetof(CDisplayManager,BackBufferCopyTexture)==0x4c);
FABLE_STATIC_ASSERT(offsetof(CDisplayManager,ShaderResourcePath)==0x258);
FABLE_STATIC_ASSERT(offsetof(CDisplayManager,MultisampleTypes)==0x2a4);
FABLE_STATIC_ASSERT(offsetof(CDisplayManager,BusyResourceManager)==0x2b0);

FABLE_STATIC_ASSERT(offsetof(CRenderManager2D,QuickDraw2DTriInfo)==0x3e94);
FABLE_STATIC_ASSERT(offsetof(CDisplayManager,ViewportOffscreen)==0x1d8);
FABLE_STATIC_ASSERT(offsetof(CDisplayManager,D3DViewport)==0x1dc);
FABLE_STATIC_ASSERT(offsetof(CDisplayManager,ViewportBox)==0x1f4);
FABLE_STATIC_ASSERT(offsetof(CDisplayManager,ViewportBoxFloat)==0x204);
