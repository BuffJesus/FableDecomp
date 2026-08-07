extern void __stdcall fl_helper(long x, int z);
struct CWorldMap { void ForceLoad(long x); };
void CWorldMap::ForceLoad(long x){ fl_helper(x, 0); }