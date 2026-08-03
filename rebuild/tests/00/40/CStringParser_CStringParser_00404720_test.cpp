#include <cstdio>
#include <cstdlib>
#include <cstring>

struct CCharString;
typedef void (*StringParserErrorCB)(CCharString*);

static int g_baseConstructCalls = 0;
static int g_charStringConstructCalls = 0;
static int g_assignCalls = 0;
static int g_initCalls = 0;
static char g_lastInitName[64];
static long g_lastInitMode = 0;
static void* g_lastAssignSelf = 0;
static const void* g_lastAssignSrc = 0;

extern "C" void __fastcall FableStringParserBaseConstruct_0099A2D0(void* self)
{
    g_baseConstructCalls++;
    *(void**)self = (void*)0x11111111;
}

extern "C" void __fastcall FableCharStringConstruct_0099E4B0(void* self)
{
    g_charStringConstructCalls++;
    *(void**)self = 0;
}

extern "C" void* __cdecl FableAllocate_00BFEA0E(unsigned int size)
{
    return malloc(size);
}

extern "C" void* __fastcall FableCharStringAssign_0099EFB0(void* self, const void* source)
{
    g_assignCalls++;
    g_lastAssignSelf = self;
    g_lastAssignSrc = source;
    *(void**)self = *(void**)const_cast<void*>(source);
    return self;
}

extern "C" void __fastcall FableStringParserInitialize_009B9530(
    void* self, char* name, StringParserErrorCB cb, void* ctx, long mode)
{
    g_initCalls++;
    if (name)
        strncpy(g_lastInitName, name, sizeof(g_lastInitName) - 1);
    else
        strcpy(g_lastInitName, "(null)");
    g_lastInitMode = mode;
    (void)self; (void)cb; (void)ctx;
}

extern "C" void* __fastcall CStringParser(
    void* self, void*, const void* src, StringParserErrorCB cb, void* ctx, long mode)
{
    unsigned char* base = (unsigned char*)self;

    FableStringParserBaseConstruct_0099A2D0(self);
    *(void**)base = (void*)0x0122d874;

    FableCharStringConstruct_0099E4B0(base + 0xc);
    void* member10 = base + 0x10;
    FableCharStringConstruct_0099E4B0(member10);

    *(void**)(base + 0x14) = 0;
    unsigned int* sentinel = (unsigned int*)FableAllocate_00BFEA0E(0x10);
    sentinel[0] = (unsigned int)sentinel;
    sentinel[1] = (unsigned int)sentinel;
    *(void**)(base + 0x14) = sentinel;
    FableCharStringAssign_0099EFB0(member10, src);

    void* srcRep = *(void**)src;
    char* name;
    if (srcRep)
        name = *(char**)srcRep;
    else
        name = (char*)0x0129aaf4;

    FableStringParserInitialize_009B9530(self, name, cb, ctx, mode);
    return self;
}

int main()
{
    unsigned char storage[0x18];
    memset(storage, 0xCD, sizeof(storage));

    char nameBuf[] = "hello";
    void* repFirstField = nameBuf;      // rep struct's first field = char* data
    void* srcRepPtr = &repFirstField;    // src's field0 ("rep") points at the rep struct
    void* srcObj = &srcRepPtr;           // src is a CCharString*: field0 = srcRepPtr

    void* result = CStringParser(storage, 0, srcObj, (StringParserErrorCB)0, (void*)0x2222, 42L);

    bool ok = true;
    ok &= (result == storage);
    ok &= (g_baseConstructCalls == 1);
    ok &= (g_charStringConstructCalls == 2);
    ok &= (g_assignCalls == 1);
    ok &= (g_initCalls == 1);
    ok &= (g_lastInitMode == 42L);
    ok &= (strcmp(g_lastInitName, "hello") == 0);
    ok &= (*(void**)storage == (void*)0x0122d874);
    ok &= (*(void**)(storage + 0x14) != 0);

    if (ok)
        printf("STRINGPARSER_CTOR_OK\n");
    else
        printf("STRINGPARSER_CTOR_FAIL\n");
    return ok ? 0 : 1;
}