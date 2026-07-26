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