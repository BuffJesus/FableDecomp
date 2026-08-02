struct IValidatable
{
    void* vtbl[4];
};

typedef void (__fastcall *ValidateFn)(void* self);

struct CMovableResourceMemoryPool
{
    void* vtbl0;

    void ValidateResource(void* p);
};

void CMovableResourceMemoryPool::ValidateResource(void* p)
{
    ValidateFn fn = (ValidateFn)(*(void***)p)[3];
    fn(p);
}