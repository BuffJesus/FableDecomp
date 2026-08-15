struct C2DVector { float x; float y; };
struct IntSize { int w; int h; };
struct MapSizeProvider { void GetSize(IntSize* other, IntSize* dst); };
struct CMiniMapDisplay { char pad[0x54]; MapSizeProvider provider; };
void __fastcall GetRelativeMapSize(CMiniMapDisplay* self, void* edx, C2DVector* out)
{
    IntSize dst;
    IntSize other;
    self->provider.GetSize(&other, &dst);
    int w=dst.w; int h=dst.h;
    if (w == h) { out->x = 1.0f; out->y = 1.0f; }
    else if (w > h) { out->x = 1.0f; out->y = (float)h / (float)w; }
    else if (w < h) { out->y = 1.0f; out->x = (float)w / (float)h; }
}