#include "fable_frontend_def.h"
#include <stdio.h>
#include <string.h>

struct AssignmentCall { int kind; const void* target; const void* source; };
static AssignmentCall g_calls[2];
static int g_count;

typedef _STL::pair<unsigned long, CDefClassBase::CSubDefInfo> SubDefPair;

template <>
_STL::vector<SubDefPair>& _STL::vector<SubDefPair>::operator=(const _STL::vector<SubDefPair>& other)
{
    if (g_count < 2) { g_calls[g_count].kind = 1; g_calls[g_count].target = this; g_calls[g_count].source = &other; }
    ++g_count;
    memcpy(this, &other, sizeof(*this));
    return *this;
}

template <>
_STL::vector<CCharString>& _STL::vector<CCharString>::operator=(const _STL::vector<CCharString>& other)
{
    if (g_count < 2) { g_calls[g_count].kind = 2; g_calls[g_count].target = this; g_calls[g_count].source = &other; }
    ++g_count;
    memcpy(this, &other, sizeof(*this));
    return *this;
}

int main()
{
    unsigned char left[sizeof(CFrontEndDef)];
    unsigned char right[sizeof(CFrontEndDef)];
    CFrontEndDef* target = reinterpret_cast<CFrontEndDef*>(left);
    CFrontEndDef* source = reinterpret_cast<CFrontEndDef*>(right);
    for (int i = 0; i < 16; ++i) {
        memset(left, 0xa5, sizeof(left));
        memset(right, 0x5a, sizeof(right));
        source->IVRefCount = 10 + i;
        source->ResourceSize = 100 + i;
        source->PDefManager = reinterpret_cast<CDefinitionManager*>(0x1000 + 16 * i);
        source->GlobalIndex = 200 + i;
        source->Setup = (i & 1) != 0;
        source->Template = (i & 2) != 0;
        source->DefaultValsApplied = (i & 4) != 0;
        source->InstantiationName.TablePos = 300 + i;
        source->SubDefInfoMap.Dirty = (i & 8) != 0;
        source->ErrorMessageBackgroundGraphic = 400 + i;
        source->ButtonABigGraphic = 500 + i;
        source->ButtonBBigGraphic = 600 + i;
        g_count = 0;
        CFrontEndDef& result = target->CFrontEndDef::operator=(*source);
        if (&result != target || g_count != 2) return 1;
        if (g_calls[0].kind != 1 || g_calls[0].target != &target->SubDefInfoMap ||
            g_calls[0].source != &source->SubDefInfoMap ||
            g_calls[1].kind != 2 || g_calls[1].target != &target->vAttractModeMovie ||
            g_calls[1].source != &source->vAttractModeMovie) return 2;
        if (memcmp(left + 4, right + 4, 32) != 0 ||
            target->Setup != source->Setup || target->Template != source->Template ||
            target->DefaultValsApplied != source->DefaultValsApplied ||
            target->InstantiationName.TablePos != source->InstantiationName.TablePos ||
            memcmp(&target->SubDefInfoMap, &source->SubDefInfoMap, 12) != 0 ||
            target->SubDefInfoMap.Dirty != source->SubDefInfoMap.Dirty ||
            memcmp(&target->vAttractModeMovie, &source->vAttractModeMovie, 12) != 0 ||
            target->ErrorMessageBackgroundGraphic != source->ErrorMessageBackgroundGraphic ||
            target->ButtonABigGraphic != source->ButtonABigGraphic ||
            target->ButtonBBigGraphic != source->ButtonBBigGraphic) return 3;
        for (int j = 0; j < 4; ++j) if (left[j] != 0xa5) return 4;
        memset(left + 4, 0xa5, sizeof(left) - 4);
        g_count = 0;
        target->CFrontEndDef::Copy(source);
        if (g_count != 2 || target->GlobalIndex != source->GlobalIndex ||
            target->InstantiationName.TablePos != source->InstantiationName.TablePos ||
            target->ButtonBBigGraphic != source->ButtonBBigGraphic) return 5;
    }
    puts("FRONTEND_DEF_ASSIGN_PASS cases=16");
    return 0;
}
