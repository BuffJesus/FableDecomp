#include <stdio.h>

struct VWindow { float x, y, z, w; };

struct CSubtitleRenderer {
    int pad0;
    VWindow vwin0;
    VWindow vwin1;
    void SetVWindowsForDeprecatedDrawInterface(const VWindow* a, const VWindow* b);
};

void CSubtitleRenderer::SetVWindowsForDeprecatedDrawInterface(const VWindow* a, const VWindow* b)
{
    this->vwin0 = *a;
    this->vwin1 = *b;
}

int main()
{
    CSubtitleRenderer obj;
    obj.pad0 = 0x11223344;
    obj.vwin0.x = obj.vwin0.y = obj.vwin0.z = obj.vwin0.w = -1.0f;
    obj.vwin1.x = obj.vwin1.y = obj.vwin1.z = obj.vwin1.w = -1.0f;

    VWindow a = { 1.0f, 2.0f, 3.0f, 4.0f };
    VWindow b = { 5.0f, 6.0f, 7.0f, 8.0f };

    obj.SetVWindowsForDeprecatedDrawInterface(&a, &b);

    if (obj.pad0 != 0x11223344) { printf("FAIL pad\n"); return 1; }
    if (obj.vwin0.x != 1.0f || obj.vwin0.y != 2.0f || obj.vwin0.z != 3.0f || obj.vwin0.w != 4.0f) { printf("FAIL vwin0\n"); return 1; }
    if (obj.vwin1.x != 5.0f || obj.vwin1.y != 6.0f || obj.vwin1.z != 7.0f || obj.vwin1.w != 8.0f) { printf("FAIL vwin1\n"); return 1; }

    printf("OK_0x0048dba6\n");
    return 0;
}