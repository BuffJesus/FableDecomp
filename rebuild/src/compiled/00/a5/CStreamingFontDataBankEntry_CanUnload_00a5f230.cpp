// CFontDataBankEntry::CanUnload  @ retail 0x00a63560
// __fastcall bool CanUnload(CFontDataBankEntry* this)

enum EFontLoadState { FONT_LOADED = 1 };

struct FontRes {
    EFontLoadState state; // [+0]
};

struct CFontDataBankEntry {
    void* vtbl;    // [+0]
    void* handle;  // [+4]
    FontRes* res;  // [+8]
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