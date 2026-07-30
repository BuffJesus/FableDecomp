
#include <stdio.h>

struct C2DPointI { int x; int y; };
struct C2DBoxI {
    int minX; int minY; int maxX; int maxY;
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

static int call_contains(const C2DBoxI* box, const C2DPointI* pt)
{
    int result;
    void* pbox = (void*)box;
    void* ppt  = (void*)pt;
    __asm {
        mov ecx, pbox
        mov eax, ppt
        push eax
        call C2DBoxI::ContainsPoint
        mov result, eax
    }
    return result;
}

int main()
{
    C2DBoxI box; box.minX = 10; box.minY = 20; box.maxX = 30; box.maxY = 40;
    C2DPointI a; a.x = 15; a.y = 25; /* inside */
    C2DPointI b; b.x = 5;  b.y = 25; /* x too small */
    C2DPointI c; c.x = 30; c.y = 25; /* x at max (exclusive) */
    C2DPointI d; d.x = 15; d.y = 10; /* y too small */
    C2DPointI e; e.x = 15; e.y = 40; /* y at max (exclusive) */
    C2DPointI f; f.x = 10; f.y = 20; /* mins inclusive */

    int ok = 1;
    if (call_contains(&box, &a) != 1) ok = 0;
    if (call_contains(&box, &b) != 0) ok = 0;
    if (call_contains(&box, &c) != 0) ok = 0;
    if (call_contains(&box, &d) != 0) ok = 0;
    if (call_contains(&box, &e) != 0) ok = 0;
    if (call_contains(&box, &f) != 1) ok = 0;

    if (ok) { printf("OK_0x004fab10\n"); return 0; }
    printf("BAD\n"); return 1;
}