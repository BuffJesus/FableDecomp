#pragma optimize("s",on)
extern void __stdcall del_helper(void* p, int a, int b);
void __stdcall operator_delete(void* p){ del_helper(p, 0, 0); }