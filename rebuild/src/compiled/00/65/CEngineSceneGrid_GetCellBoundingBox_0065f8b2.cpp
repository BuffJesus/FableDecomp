#pragma optimize("s",on)
// CEngineSceneGrid::GetCellBoundingBox  retail 0065f8b2 (24 bytes)
// EBP frame, one local bool, forward to __cdecl helper(a,b,&local),
// add esp,0xc (helper cleans its 3 args), leave; ret (bare -> caller-clean).
// Reaches byte-parity under size-optimized codegen (#pragma optimize("s",on)).

struct CEngineSceneGrid;

extern "C" char __cdecl helper_65e8b8(unsigned long a, unsigned long b, char* out);

char __cdecl GetCellBoundingBox(unsigned long a, unsigned long b)
{
    char r;
    return helper_65e8b8(a, b, &r);
}