// CMap::DrawGetSeaLevel @ 0x004bbff0
// Disassembly:
//   fld  dword ptr [ecx+0x4c]
//   fmul dword ptr [ecx+0x34]
//   ret
// __fastcall returning float: this->f4c * this->f34
struct CMap {
    char pad00[0x34];
    float f34;      // +0x34 scale factor
    char pad38[0x14];
    float f4c;      // +0x4c sea level
};

float __fastcall DrawGetSeaLevel(CMap *thisptr)
{
    return thisptr->f4c * thisptr->f34;
}