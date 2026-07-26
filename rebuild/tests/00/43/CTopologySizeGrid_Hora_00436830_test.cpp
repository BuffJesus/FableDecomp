#include <cstdio>

struct CTopologySizeGrid {
    int dummy0;
    int* p4;    // +0x4
    int pad8;
    int padC;
    int pad10;
    int* p14;   // +0x14
};

float* __fastcall CTopologySizeGrid_Hora(CTopologySizeGrid* self, int edx_unused, unsigned char i, int b, int c)
{
    unsigned int idx = (unsigned char)i;
    int* rowbase = self->p4;
    int* ptrs = self->p14;
    int t = rowbase[idx * 5 + 1] * c;
    int base = ptrs[idx * 3];
    t = t + b;
    return (float*)(base + t * 4);
}

int main()
{
    // Build a rowbase array (int[20]) and a ptrs array.
    static int rowbase[20];
    static int ptrs[10];
    static float storage[64];

    // i = 2 -> rowbase[2*5+1] = rowbase[11], ptrs[2*3] = ptrs[6]
    rowbase[11] = 3;   // multiplier
    ptrs[6] = (int)(&storage[0]);

    CTopologySizeGrid g;
    g.p4 = rowbase;
    g.p14 = ptrs;

    unsigned char i = 2;
    int b = 5;
    int c = 4;

    // expected t = rowbase[11]*c + b = 3*4 + 5 = 17
    // return = base + 17*4 = &storage[17]
    float* r = CTopologySizeGrid_Hora(&g, 0, i, b, c);
    float* expected = &storage[17];

    if (r == expected) {
        std::printf("CTopologySizeGrid_00436830_TEST PASS\n");
        return 0;
    }
    std::printf("FAIL got=%p expected=%p\n", (void*)r, (void*)expected);
    return 1;
}