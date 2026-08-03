#pragma optimize("y", off)
#pragma optimize("s", on)

// CEngineLightingManager::UpdateShadowScene @ 0x00473593  (__fastcall, ret 4)
//
// Retail idiom:
//   push ebp; mov ebp,esp; push ecx          -> 1 local byte at [ebp-4]
//   push esi; push 0x122d70e; mov esi,ecx     -> save this=esi; push tag
//   call <Prep>                               -> __stdcall Prep(0x122d70e)
//   mov eax,[esi+0x18]; dec eax; dec eax       -> switch(this->mode)
//   mov byte[ebp-4],0                          -> local.flag = 0
//   je case2; dec eax; jne end                 -> case 2 / case 3 dispatch
//   case3: movzx eax,byte[p]; ecx=[esi+0x28]; push; call  -> sink28->Take((unsigned char)*p)
//   case2: push p; lea ecx,[ebp-4]; push [esi+0x24]; call -> local.Fill(this->obj24, p)
//   pop esi; leave; ret 4

struct Sink {
    void Take(int v);            // __fastcall, ecx=this, one dword arg
};

struct Local {
    unsigned char flag;          // [ebp-4] byte, set to 0
    void Fill(int a, void* b);   // __fastcall, ecx=&local, two dword args
};

class CEngineLightingManager {
public:
    char  pad00[0x18];
    int   mode;                  // +0x18
    char  pad1C[0x08];
    int   obj24;                 // +0x24
    Sink* sink28;                // +0x28
    void UpdateShadowScene(unsigned char* p);
};

extern void __stdcall Prep(unsigned int tag);   // push 0x122d70e; call

void CEngineLightingManager::UpdateShadowScene(unsigned char* p)
{
    Local h;
    Prep(0x122d70eU);
    h.flag = 0;
    switch (this->mode) {
    case 2:
        h.Fill(this->obj24, p);
        break;
    case 3:
        this->sink28->Take(*p);
        break;
    }
}