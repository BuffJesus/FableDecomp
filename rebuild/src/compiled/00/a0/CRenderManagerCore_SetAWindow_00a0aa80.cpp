struct C2DBoxF;

struct CDisplayManagerWindowView
{
    void SetViewport(const C2DBoxF& viewport);
};

struct CRenderManagerCoreWindowView
{
    unsigned char unknown0000[0x3A3C];
    CDisplayManagerWindowView* displayManager3A3C;

    void SetAWindow(const C2DBoxF& window);
};

void CRenderManagerCoreWindowView::SetAWindow(
    const C2DBoxF& window)
{
    displayManager3A3C->SetViewport(window);
}
