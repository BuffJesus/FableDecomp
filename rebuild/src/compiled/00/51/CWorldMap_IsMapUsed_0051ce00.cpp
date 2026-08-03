// CWorldMap::IsMapUsed @ 0x0051ce00
// Retail: mov eax,[esp+4]; mov ecx,[ecx+0x20]; lea eax,[eax+eax*8]; mov al,[ecx+eax*8+0x24]; ret 4
// __fastcall(this=ecx) with one 32-bit stack arg (map index). The map table
// base pointer lives at this+0x20; each entry is 0x48 (72) bytes
// (9*8 = index*9 scaled by *8 in the addressing mode), and the 'used' flag
// byte sits at entry+0x24. Returns that byte (a bool). Authored as an
// explicit __fastcall with a dummy edx slot so the index stays a STACK arg
// (matching __fastcall's ABI: this in ecx, first real arg on the stack).
struct MapEntry { char pad[0x24]; unsigned char used; char pad2[0x48-0x25]; };
struct CWorldMap { char pad[0x20]; MapEntry* maps; };
unsigned char __fastcall IsMapUsed(CWorldMap* self, int /*edx*/, int index)
{
    return self->maps[index].used;
}