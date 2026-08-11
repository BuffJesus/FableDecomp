#include <cstdio>

struct CTCBase;

struct TList {
    void* head;
    void remove(CTCBase* const& val);
};

struct CTCInventoryAbilities { char pad0[0x30]; TList learned; };

static TList*   g_self;
static CTCBase* g_val;

void TList::remove(CTCBase* const& val)
{
    g_self = this;
    g_val  = val;
}

void __fastcall AddLearnedExpression(CTCInventoryAbilities* self, void* edx, long a);

int main()
{
    CTCInventoryAbilities obj;
    obj.learned.head = (void*)0x22222222;

    long a = 12345;
    AddLearnedExpression(&obj, 0, a);

    bool ok = (g_self == &obj.learned) && (g_val == (CTCBase*)a);
    if (ok) printf("00c21f30_TEST PASS\n");
    else    printf("FAIL self=%p val=%p\n", (void*)g_self, (void*)g_val);
    return 0;
}