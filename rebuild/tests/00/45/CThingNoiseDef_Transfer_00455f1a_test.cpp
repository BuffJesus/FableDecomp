#include <cstdio>

static int g_base = 0;
static const void* g_name = 0;
static void* g_field = 0;
static void* g_ar = 0;

struct CArchive {
    void Transfer(const void* name, void* field) { g_name = name; g_field = field; g_ar = this; }
};
struct CThingDef {
    void Transfer(CArchive* ar) { g_base = 1; g_ar = ar; }
};
struct CThingNoiseDef : public CThingDef {
    char extra[0x70];
    int noise;
    void Transfer(CArchive* ar);
};

const char kName[] = "CThingNoiseDef";

void CThingNoiseDef::Transfer(CArchive* ar)
{
    CThingDef::Transfer(ar);
    ar->Transfer(kName, &this->noise);
}

int main() {
    CThingNoiseDef obj;
    CArchive arobj;
    obj.Transfer(&arobj);
    if (g_base == 1 && g_name == kName && g_field == &obj.noise && g_ar == &arobj) {
        std::printf("CThingNoiseDef_00455f1a_TEST PASS\n");
        return 0;
    }
    std::printf("FAIL\n");
    return 1;
}