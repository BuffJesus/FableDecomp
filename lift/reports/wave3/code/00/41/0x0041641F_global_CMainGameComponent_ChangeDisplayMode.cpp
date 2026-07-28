#include <cstddef>
#include <cstdint>

struct CMainGameComponent;
struct CDisplayManager;
struct CSurface;
struct CPixelFormat;
struct CScriptThing;

struct C2DExtentsI
{
    std::uint32_t x;
    std::uint32_t y;
};

struct CMainGameComponent_Overlay
{
    std::byte pad_00[0x28];
    CScriptThing* scriptThing;
};

static_assert(offsetof(CMainGameComponent_Overlay, scriptThing) == 0x28);

struct CDisplayManager_Overlay
{
    std::byte pad_00[0x2C];
    std::byte primarySurface;
};

static_assert(offsetof(CDisplayManager_Overlay, primarySurface) == 0x2C);

extern CDisplayManager* DAT_013b8390;

void __thiscall CMainGameComponent_ChangeDisplayMode(
    CMainGameComponent* self,
    std::uint32_t width,
    std::uint32_t height,
    std::uint8_t displayMode)
{
#if defined(__GNUC__) || defined(__clang__)
    register CScriptThing* unaff_EDI __asm__("edi");
#else
#   error This reconstruction requires a named x86 register binding for unaff_EDI.
#endif

    CScriptThing::operator==(
        reinterpret_cast<CMainGameComponent_Overlay*>(self)->scriptThing,
        unaff_EDI);

    C2DExtentsI extents{width, height};

    CDisplayManager::SetDisplayMode(
        DAT_013b8390,
        &extents,
        static_cast<std::uint32_t>(displayMode),
        0,
        0);

    auto* const displayManager =
        reinterpret_cast<CDisplayManager_Overlay*>(DAT_013b8390);
    auto* const surface =
        reinterpret_cast<CSurface*>(&displayManager->primarySurface);

    const auto dimensions =
        static_cast<unsigned long*>(CSurface::GetDimensions(surface));
    const unsigned long surfaceWidth = *dimensions;

    auto* const pixelFormat =
        static_cast<CPixelFormat*>(CSurface::GetPixelFormat(surface));
    const unsigned long colourDepth =
        CPixelFormat::GetColourDepth(pixelFormat);

    const unsigned long surfaceHeight = CSurface::GetHeight(surface);

    CMainGameComponent::PostChangeResolution(
        self,
        surfaceWidth,
        surfaceHeight,
        colourDepth);
}