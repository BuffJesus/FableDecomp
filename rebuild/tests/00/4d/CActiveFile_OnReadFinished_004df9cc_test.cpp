#include <cstdio>
struct AutoTinyActiveFile
{
    void* resource;
    void* reader;
};
static int g_AutoTinyFinishReadCalls = 0;
static int g_AutoTinyReleaseResourceCalls = 0;
static void* g_AutoTinyFinishedResource = 0;
static void* g_AutoTinyFinishedReader = 0;
void __fastcall AutoTinyFinishRead(void* resource, void* reader, char* status)
{
    ++g_AutoTinyFinishReadCalls;
    g_AutoTinyFinishedResource = resource;
    g_AutoTinyFinishedReader = reader;
    *status = 1;
}
void __cdecl AutoTinyReleaseResource(void*)
{
    ++g_AutoTinyReleaseResourceCalls;
}
void __fastcall OnReadFinished(AutoTinyActiveFile* self)
{
    char status;
    AutoTinyFinishRead(self->resource, self->reader, &status);
    if (self->resource != 0)
        AutoTinyReleaseResource(self->resource);
}
int main()
{
    int resource = 0;
    int reader = 0;
    AutoTinyActiveFile present = {&resource, &reader};
    OnReadFinished(&present);
    if (g_AutoTinyFinishReadCalls != 1 ||
        g_AutoTinyReleaseResourceCalls != 1 ||
        g_AutoTinyFinishedResource != &resource ||
        g_AutoTinyFinishedReader != &reader)
        return 1;
    AutoTinyActiveFile absent = {0, &reader};
    OnReadFinished(&absent);
    if (g_AutoTinyFinishReadCalls != 2 ||
        g_AutoTinyReleaseResourceCalls != 1)
        return 1;
    std::printf("AUTO_TINY_004df9cc_TEST PASS\n");
    return 0;
}