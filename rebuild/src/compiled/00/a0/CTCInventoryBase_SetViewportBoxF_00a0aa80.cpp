struct C2DBoxF;

struct CDisplayManagerViewportBoxFView
{
    void SetViewport(const C2DBoxF& viewport);
};

struct CTCInventoryBaseViewportBoxFView
{
    unsigned char unknown0000[0x3A3C];
    CDisplayManagerViewportBoxFView* displayManager3A3C;

    void SetViewport(const C2DBoxF& viewport);
};

void CTCInventoryBaseViewportBoxFView::SetViewport(
    const C2DBoxF& viewport)
{
    displayManager3A3C->SetViewport(viewport);
}
