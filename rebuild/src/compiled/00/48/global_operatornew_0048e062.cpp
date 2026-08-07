#pragma optimize("s",on)
struct Pool { void* Alloc(unsigned s); };
void* __stdcall operator_new(unsigned size, Pool* pool){ return pool->Alloc(size); }