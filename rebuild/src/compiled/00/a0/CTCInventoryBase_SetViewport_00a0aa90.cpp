struct C2DBoxI;

struct CDisplayManager
{
    void SetViewport(const C2DBoxI& viewport);
};

struct CTCInventoryBaseViewportView
{
    unsigned char unknown0000[0x3A3C];
    CDisplayManager* displayManager3A3C;

    void SetViewport_00A0AA90(const C2DBoxI& viewport);
};

void CTCInventoryBaseViewportView::SetViewport_00A0AA90(
    const C2DBoxI& viewport)
{
    displayManager3A3C->SetViewport(viewport);
}
