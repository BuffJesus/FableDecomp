
struct C2DPointI { int x; int y; };

struct C2DBoxI {
    int minX; // +0x0
    int minY; // +0x4
    int maxX; // +0x8
    int maxY; // +0xc
    int ContainsPoint(const C2DPointI* p) const;
};

int C2DBoxI::ContainsPoint(const C2DPointI* p) const
{
    int x = p->x;
    if (x >= minX && x < maxX)
    {
        int y = p->y;
        if (y >= minY && y < maxY)
            return 1;
    }
    return 0;
}