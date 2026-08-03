#pragma optimize("s",on)
struct VWindow { float x, y, z, w; };

struct CSubtitleRenderer {
    int pad0;         // +0x00
    VWindow vwin0;    // +0x04
    VWindow vwin1;    // +0x14
    void SetVWindowsForDeprecatedDrawInterface(const VWindow* a, const VWindow* b);
};

void CSubtitleRenderer::SetVWindowsForDeprecatedDrawInterface(const VWindow* a, const VWindow* b)
{
    this->vwin0 = *a;
    this->vwin1 = *b;
}