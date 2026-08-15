#include <cstdio>

struct C2DVector { float x; float y; };

static void model(int w, int h, C2DVector* out)
{
    if (w == h) { out->x = 1.0f; out->y = 1.0f; }
    else if (w > h) { out->x = 1.0f; out->y = (float)h / (float)w; }
    else if (w < h) { out->y = 1.0f; out->x = (float)w / (float)h; }
}

int main()
{
    C2DVector v;
    int ok = 1;

    model(100, 100, &v);
    if (!(v.x == 1.0f && v.y == 1.0f)) ok = 0;

    model(200, 100, &v); // w>h
    if (!(v.x == 1.0f && v.y == 0.5f)) ok = 0;

    model(100, 200, &v); // w<h
    if (!(v.y == 1.0f && v.x == 0.5f)) ok = 0;

    if (ok) printf("PARITY_OK\n");
    else printf("PARITY_FAIL\n");
    return 0;
}