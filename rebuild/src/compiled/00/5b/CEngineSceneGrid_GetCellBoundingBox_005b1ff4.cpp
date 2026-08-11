#pragma optimize("s",on)
#pragma optimize("y", off)

struct CEngineSceneGrid {};

// __cdecl helper at 0x5b1afd: (ulong, ulong, out ptr) -> bool
extern bool helper_5b1afd(unsigned long a, unsigned long b, void* out);

bool __cdecl GetCellBoundingBox(unsigned long a, unsigned long b)
{
    char local;
    return helper_5b1afd(a, b, &local);
}