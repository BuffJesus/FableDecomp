#include <cstddef>
#include <cstdint>

struct ComCreateSourceVTable;
struct ComCreateHostVTable;
struct UnknownReleasableVTable;

struct ComCreateSource
{
    ComCreateSourceVTable* vtbl;
};

struct ComCreateHost
{
    ComCreateHostVTable* vtbl;
};

struct UnknownReleasable
{
    UnknownReleasableVTable* vtbl;
};

struct ComCreateSourceVTable
{
    std::byte pad00[0x0C];
    int (__thiscall* Create)(ComCreateSource* self, void** outObject);                 // +0x0C
    int (__thiscall* Activate)(ComCreateSource* self, void* object, int zeroFlag);    // +0x10
    int (__thiscall* InitCreateContext)(ComCreateSource* self, void* createContext);  // +0x14
};

struct ComCreateHostVTable
{
    std::byte pad00[0x3C];
    int (__thiscall* Bind)(ComCreateHost* self, void* object, void* bindArg);         // +0x3C
    int (__thiscall* CreateFallback)(ComCreateHost* self, void** outObject);          // +0x48
};

struct UnknownReleasableVTable
{
    std::byte pad00[0x08];
    void (__thiscall* Release)(UnknownReleasable* self);                               // +0x08
};

static_assert(offsetof(ComCreateSourceVTable, Create) == 0x0C);
static_assert(offsetof(ComCreateSourceVTable, Activate) == 0x10);
static_assert(offsetof(ComCreateSourceVTable, InitCreateContext) == 0x14);
static_assert(offsetof(ComCreateHostVTable, Bind) == 0x3C);
static_assert(offsetof(ComCreateHostVTable, CreateFallback) == 0x48);
static_assert(offsetof(UnknownReleasableVTable, Release) == 0x08);

int __thiscall Com_CreateWithFallback(ComCreateHost* self, ComCreateSource* source, void** outObject)
{
    int createContext[4] = {};
    void* bindArg0 = createContext;
    void* bindArg1 = source;
    int result;

    *outObject = nullptr;

    source->vtbl->InitCreateContext(source, &createContext[0]);
    if (createContext[0] == 0)
    {
        createContext[0] = 1;
    }

    result = source->vtbl->Create(source, outObject);
    if (result >= 0)
    {
        result = self->vtbl->Bind(self, *outObject, &bindArg0);
        if (result >= 0)
        {
            result = source->vtbl->Activate(source, *outObject, 0);
            if (result >= 0)
            {
                return 0;
            }
        }
    }

    if (*outObject != nullptr)
    {
        static_cast<UnknownReleasable*>(*outObject)->vtbl->Release(
            static_cast<UnknownReleasable*>(*outObject));
        *outObject = nullptr;
    }

    result = self->vtbl->CreateFallback(self, outObject);
    if (result >= 0)
    {
        result = self->vtbl->Bind(self, *outObject, &bindArg1);
        if (result >= 0)
        {
            result = source->vtbl->Activate(source, *outObject, 0);
            if (result >= 0)
            {
                return 0;
            }
        }
    }

    if (*outObject != nullptr)
    {
        static_cast<UnknownReleasable*>(*outObject)->vtbl->Release(
            static_cast<UnknownReleasable*>(*outObject));
        *outObject = nullptr;
    }

    return result;
}