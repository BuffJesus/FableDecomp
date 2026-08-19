#pragma optimize("s",on)
// BeginInputLoading — forwards to a member of a fixed global object with a fixed
// name argument. __stdcall, one stack arg (ret 4); ecx comes from the global, so the
// function ignores any incoming this.
struct CLoader { void Begin(const char* name, int arg); };
extern CLoader* g_Loader;    // 0x013B8388
extern const char g_Name[];  // 0x013B85E4
void __stdcall BeginInputLoading(int arg) { g_Loader->Begin(g_Name, arg); }