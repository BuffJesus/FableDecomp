extern "C" void* __cdecl helper(unsigned int n);
void* __stdcall operator_new(unsigned int n){ return helper(n); }