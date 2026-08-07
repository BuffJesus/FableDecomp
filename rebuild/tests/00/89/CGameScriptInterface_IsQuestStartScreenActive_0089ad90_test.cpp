#include <stdio.h>

class CCharStringData
{
public:
    char* m_psz;
    long m_nLength;
};

class CCharString
{
public:
    CCharStringData* m_pString;
};

struct Pair
{
    long m_Key;
    void* m_Value;
};

static Pair* g_lowerBoundResult;

struct InterfaceMap
{
    Pair* m_pBegin;
    Pair* m_pEnd;

    Pair* LowerBound(const long*)
    {
        return g_lowerBoundResult;
    }
};

struct Registry
{
    unsigned char m_Pad00[0x3C];
    unsigned long m_Flags3C;
    unsigned char m_Pad40[0x04];
    InterfaceMap m_Map44;
};

struct CTCBase
{
    unsigned char m_Pad00[0x0C];
    unsigned char m_Flag0C;
    unsigned char m_Pad0D[0x0F];
    unsigned char m_Flag1C;
    unsigned char m_Pad1D[0x03];
    unsigned long m_Value20;
    unsigned char m_Pad24[0x2C];
    CCharStringData* m_String50;
};

class CGameScriptInterface
{
public:
    bool IsQuestStartScreenActive(CCharString& value) const;
};

static Registry* g_registry;
static int g_compareCalls;

extern "C" void* __fastcall Helper00449970(void* value)
{
    return value;
}

extern "C" Registry* __fastcall Helper00487DC0(void*)
{
    return g_registry;
}

long __fastcall Compare00411570(const char* lhs, const char* rhs)
{
    ++g_compareCalls;
    while (*lhs != 0 && *lhs == *rhs)
    {
        ++lhs;
        ++rhs;
    }
    return (unsigned char)*lhs - (unsigned char)*rhs;
}

bool CGameScriptInterface::IsQuestStartScreenActive(CCharString& value) const
{
    Registry* registry = Helper00487DC0(Helper00449970(g_registry));
    if ((registry->m_Flags3C & 0x10000000UL) != 0)
    {
        long questStartScreenType = 0xFC;
        Pair* entry = registry->m_Map44.LowerBound(&questStartScreenType);
        if (entry == registry->m_Map44.m_pEnd || 0xFC < entry->m_Key)
            entry = registry->m_Map44.m_pEnd;
        CTCBase* tcBase = (CTCBase*)entry->m_Value;
        if (tcBase->m_Flag0C != 0 &&
            tcBase->m_Flag1C == 0 &&
            tcBase->m_Value20 == 0)
        {
            CCharStringData* lhs = tcBase->m_String50;
            CCharStringData* rhs = value.m_pString;
            if (lhs == rhs)
                return true;
            if (lhs != 0 && rhs != 0 && lhs->m_nLength == rhs->m_nLength)
                return Compare00411570(lhs->m_psz, rhs->m_psz) == 0;
        }
    }
    return false;
}

int main()
{
    Registry registry;
    Pair entry;
    Pair end;
    CTCBase base;
    CCharStringData stored;
    CCharStringData equal;
    CCharStringData different;
    CCharString name;
    char storedText[] = "quest_start";
    char equalText[] = "quest_start";
    char differentText[] = "other";
    CGameScriptInterface object;

    g_registry = &registry;
    g_lowerBoundResult = &entry;
    registry.m_Flags3C = 0x10000000UL;
    registry.m_Map44.m_pEnd = &end;
    entry.m_Key = 0xFC;
    entry.m_Value = &base;
    end.m_Key = 0;
    end.m_Value = &base;
    base.m_Flag0C = 1;
    base.m_Flag1C = 0;
    base.m_Value20 = 0;
    base.m_String50 = &stored;
    stored.m_psz = storedText;
    stored.m_nLength = 11;
    equal.m_psz = equalText;
    equal.m_nLength = 11;
    different.m_psz = differentText;
    different.m_nLength = 5;

    name.m_pString = &stored;
    if (!object.IsQuestStartScreenActive(name))
        return 1;
    name.m_pString = &equal;
    if (!object.IsQuestStartScreenActive(name) || g_compareCalls != 1)
        return 2;
    name.m_pString = &different;
    if (object.IsQuestStartScreenActive(name))
        return 3;
    registry.m_Flags3C = 0;
    if (object.IsQuestStartScreenActive(name))
        return 4;

    printf("FSE2_0089ad90_TEST PASS\n");
    return 0;
}
