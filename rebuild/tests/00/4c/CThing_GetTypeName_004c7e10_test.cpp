#include <cstdio>
#include <cstring>

struct CThingTypeEntry { int a,b,c,d; char name[16]; };
struct CThingTypeManager { int pad; CThingTypeEntry *entries; };
CThingTypeManager *g_thingTypeManager;
struct CThing { char pad[0x10]; unsigned char typeIndex; };

char * __fastcall GetTypeName(CThing *self)
{
    unsigned int idx = self->typeIndex;
    return g_thingTypeManager->entries[idx].name;
}

int main()
{
    // Build a small type table.
    static CThingTypeEntry table[4];
    strcpy(table[0].name, "CREATURE");
    strcpy(table[1].name, "OBJECT");
    strcpy(table[2].name, "MARKER");
    strcpy(table[3].name, "REGION");
    static CThingTypeManager mgr;
    mgr.entries = table;
    g_thingTypeManager = &mgr;

    CThing t;
    memset(&t, 0, sizeof(t));
    t.typeIndex = 2;   // expect "MARKER"

    char *got;
    CThing *pt = &t;
    __asm {
        mov ecx, pt
        call GetTypeName
        mov got, eax
    }

    // The returned pointer must be exactly &table[2].name
    if (got != table[2].name) { printf("FAIL ptr mismatch\n"); return 1; }
    if (strcmp(got, "MARKER") != 0) { printf("FAIL name=%s\n", got); return 1; }

    // Also verify index 0 boundary.
    t.typeIndex = 0;
    __asm {
        mov ecx, pt
        call GetTypeName
        mov got, eax
    }
    if (got != table[0].name || strcmp(got, "CREATURE") != 0) { printf("FAIL idx0\n"); return 1; }

    printf("OK_0x004c7e10 name=%s\n", table[2].name);
    return 0;
}