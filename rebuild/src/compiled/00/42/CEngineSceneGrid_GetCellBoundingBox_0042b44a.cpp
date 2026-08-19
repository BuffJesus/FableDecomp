#pragma optimize("s",on)
// CEngineSceneGrid::GetCellBoundingBox  retail 005b1975 (24 bytes)
// EBP frame, one local bool, forward to __cdecl helper(a,b,&local),
// add esp,0xc (helper cleans its 3 args), leave; ret (bare -> caller-clean).

struct CEngineSceneGrid;

extern "C" char __cdecl helper_5b155a(unsigned long a, unsigned long b, char* out);

char __cdecl GetCellBoundingBox(unsigned long a, unsigned long b)
{
    char r;
    return helper_5b155a(a, b, &r);
}