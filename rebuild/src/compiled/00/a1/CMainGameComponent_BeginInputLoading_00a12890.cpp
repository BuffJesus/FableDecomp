extern void __stdcall bil_helper(void* p, int z);
struct CMainGameComponent { void BeginInputLoading(void* a); };
void CMainGameComponent::BeginInputLoading(void* a){ bil_helper(a, 0); }