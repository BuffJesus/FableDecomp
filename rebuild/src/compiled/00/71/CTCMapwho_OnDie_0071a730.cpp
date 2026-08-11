// CTCMapwho::OnDie @ 0071a730

struct CTCMapwho;

struct Helper2 {
    void Notify(CTCMapwho* thing);  // __fastcall: ecx=this(that), stack arg=thing
};

struct HelperObj {
    char pad[0x10];
    Helper2* f10; // +0x10
};

struct CTCMapwho {
    char pad[0x50];
    unsigned char f50; // +0x50
};

// helper_686d30: no-arg call returning a HelperObj*
extern HelperObj* helper_686d30(void);

void __fastcall CTCMapwho_OnDie(CTCMapwho* self)
{
    if (self->f50 & 1) {
        HelperObj* x = helper_686d30();
        x->f10->Notify(self);
    }
}