#include <cstdio>

enum EFontLoadState { FONT_LOADED = 1 };

struct FontRes {
    EFontLoadState state;
};

struct CFontDataBankEntry {
    void* vtbl;
    void* handle;
    FontRes* res;
    bool CanUnload();
};

bool CFontDataBankEntry::CanUnload()
{
    if (this->handle != 0 && this->res != 0)
    {
        int s = this->res->state;
        if (s == FONT_LOADED)
            return s == FONT_LOADED;
    }
    return false;
}

int main()
{
    FontRes r; r.state = FONT_LOADED;
    int dummy = 0;
    CFontDataBankEntry e;
    e.vtbl = 0; e.handle = &dummy; e.res = &r;

    bool a = e.CanUnload();                 // true

    r.state = (EFontLoadState)0;
    bool b = e.CanUnload();                  // false

    r.state = (EFontLoadState)2;
    bool c = e.CanUnload();                  // false

    e.res = 0; r.state = FONT_LOADED;
    bool d = e.CanUnload();                  // false

    e.res = &r; e.handle = 0;
    bool f = e.CanUnload();                  // false

    if (a && !b && !c && !d && !f) {
        printf("CANUNLOAD_OK\n");
        return 0;
    }
    printf("CANUNLOAD_FAIL\n");
    return 1;
}