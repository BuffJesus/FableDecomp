// CopyBackBufferToTexture @ 0x004df8e9
// __fastcall CTexture* (CDisplayManager*) -- modeled as __fastcall(self)
class CTexture;
class CDisplayManager;

// helper @ 0x4dd383 (__stdcall, callee-cleaned, two byte-buffer pointers)
void __stdcall helper_4dd383(char* a, char* b);

#pragma optimize("s", on)
CTexture* __fastcall CopyBackBufferToTexture(CDisplayManager* self)
{
    char* pa;
    char* pb;
    { char a; pa = &a; }
    { char b; pb = &b; }
    helper_4dd383(pa, pb);
    return (CTexture*)self;
}