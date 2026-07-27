struct C2DBoxF
{
    float left;
    float top;
    float right;
    float bottom;
};

struct C2DBoxI
{
    int left;
    int top;
    int right;
    int bottom;
};

void __fastcall
GFVBoxToABox(
    const C2DBoxF* source,
    C2DBoxI* destination);

struct CDisplayManager
{
    void SetViewport(const C2DBoxI& viewport);
};

struct CTCInventoryBaseViewportFloatView
{
    unsigned char unknown0000[0x3A3C];
    CDisplayManager* displayManager3A3C;

    void SetViewportFloat_00A0B560(const C2DBoxF& viewport);
};

void CTCInventoryBaseViewportFloatView::SetViewportFloat_00A0B560(
    const C2DBoxF& viewport)
{
    C2DBoxI integerViewport;
    GFVBoxToABox(&viewport, &integerViewport);
    displayManager3A3C->SetViewport(integerViewport);
}
