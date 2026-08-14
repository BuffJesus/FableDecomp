typedef int D3DFORMAT;

extern int g_formatCursor[]; // symbol address == 0x129ba48

long __fastcall FindFormatInfoIndexFromD3DFormat(D3DFORMAT fmt)
{
    long index = 0;
    int* edx = &g_formatCursor[0];
top:
    if (edx[-2] == (int)fmt)
        return index;
    edx += 9;              // advance to next record cursor
    ++index;
    if (edx[0] != -1)      // marker at new cursor (== old edx + 0x24)
        goto top;
    return -1;
}