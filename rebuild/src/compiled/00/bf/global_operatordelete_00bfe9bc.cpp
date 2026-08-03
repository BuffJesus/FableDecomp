extern "C" void __cdecl free(void*);
typedef void (__cdecl *FreeFn)(void*);
extern FreeFn __imp_free;
FreeFn __imp_free = &free;
void __cdecl operator_delete(void* p)
{
    __imp_free(p);
}