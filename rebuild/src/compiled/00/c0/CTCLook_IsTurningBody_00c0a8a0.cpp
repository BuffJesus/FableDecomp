// CTCLook::IsTurningBody @ 0x00c0a8a0
// __fastcall on ecx=this; two dword stack args A,B (ret 8).
// mov dl,[esp+8]=B(byte) ; lea eax,this+0x2a ; add ecx,0x4c(this+0x4c) ; push eax;push ecx;
// mov ecx,[esp+0xc] -> after 2 pushes = original [esp+4] = A ; call helper ; ret 8.
// So helper(ecx=A, edx=B(byte), stack: this+0x4c, this+0x2a).
// helper @ 0x00c0aee0 : __fastcall(void* self=A, char b=B, void* p1=this+0x4c, void* p2=this+0x2a)

struct CTCLook;

extern bool __fastcall Helper_00c0aee0(void* self, char b, void* p1, void* p2);

// __fastcall self->ecx. A->[esp+4], B->[esp+8].  (harness rewrites __fastcall->__fastcall,
// so declare the edx dummy explicitly to keep this-in-ecx + two stack params + ret 8.)
bool __fastcall IsTurningBody(CTCLook* self, int /*edx*/, void* A, char B)
{
    char* base = (char*)self;
    return Helper_00c0aee0(A, B, base + 0x4c, base + 0x2a);
}