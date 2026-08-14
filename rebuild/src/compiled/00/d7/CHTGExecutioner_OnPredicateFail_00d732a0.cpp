// Byte-exact reconstruction of CHTG_Executioner::OnPredicateFail @ 0x00d732a0

// The pushed arg is the ADDRESS 0x12d29d4 (a global). Model as extern global,
// pass its address.
extern char g_PredMsg;

// Helper subobject living at obj+0x9c; its method takes a void* stack arg.
// Native member => VC7.1 __fastcall (ecx=this, msg on stack, no edx).
struct SubObj {
    void Report(void* msg);
};

struct Obj {
    char pad[0x9c];
    // SubObj begins at +0x9c (flag byte at +0x4b is within pad)
};

struct CHTG_Executioner {
    char pad[0x14];
    Obj* obj;   // +0x14
};

void __fastcall OnPredicateFail(CHTG_Executioner* self)
{
    SubObj* s = (SubObj*)((char*)self->obj + 0x9c);
    s->Report(&g_PredMsg);
    ((char*)self->obj)[0x4b] = 1;
}