#include <cstdio>

struct CPersistTraits_CReactionFrequencyTraitsArray_
{
    void* field0;
};

static void* g_seen = 0;
void __stdcall PersistTraitsHelper(void* p) { g_seen = p; }

void __fastcall dtor_CPersistTraits_CReactionFrequencyTraitsArray_(
        CPersistTraits_CReactionFrequencyTraitsArray_* self);

int main()
{
    int dummy = 0x1234;
    CPersistTraits_CReactionFrequencyTraitsArray_ obj;
    obj.field0 = &dummy;
    dtor_CPersistTraits_CReactionFrequencyTraitsArray_(&obj);
    if (g_seen == &dummy)
        printf("PERSISTTRAITS_DTOR_OK\n");
    else
        printf("FAIL\n");
    return (g_seen == &dummy) ? 0 : 1;
}