struct V4 { int a, b, c, d; };

struct GFScreen {
    int a, b, c, d;
    GFScreen* GFToScreen(const V4* src);
};

GFScreen* GFScreen::GFToScreen(const V4* src)
{
    a = src->a;
    b = src->b;
    c = src->c;
    d = src->d;
    return this;
}